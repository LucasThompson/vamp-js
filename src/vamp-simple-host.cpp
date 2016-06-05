#include <vamp-hostsdk/Plugin.h>
#include <vamp-hostsdk/PluginHostAdapter.h>
#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginChannelAdapter.h>

#include <iostream>
#include <fstream>
#include <set>
#include <sndfile.h>

#include <cstring>
#include <cstdlib>

#include "system.h"

#include <cmath>
#include "PowerSpectrum.h"


using namespace std;

using Vamp::Plugin;
using Vamp::PluginHostAdapter;
using Vamp::RealTime;
using Vamp::HostExt::PluginChannelAdapter;
using Vamp::HostExt::PluginWrapper;
using Vamp::HostExt::PluginInputDomainAdapter;

void printFeatures(int, int, int, Plugin::FeatureSet, ofstream *, bool frames);
int runPlugin(string myname, string output, int outputNo, string inputFile, string outfilename, bool frames);
void adaptSafe(Plugin** plugin);

void adaptSafe(Plugin** plugin)
{
    if ((*plugin)->getInputDomain() == Plugin::FrequencyDomain) {
        *plugin = new PluginInputDomainAdapter(*plugin);
    }
    *plugin = new PluginChannelAdapter(*plugin);
}

int runPlugin(string myname, Plugin* plugin, string output, int outputNo, string inputFile,
              bool useFrames)
{
    
    SNDFILE *sndfile;
    SF_INFO sfinfo;
    memset(&sfinfo, 0, sizeof(SF_INFO));

    sndfile = sf_open(inputFile.c_str(), SFM_READ, &sfinfo);
    if (!sndfile) {
        cerr << myname << ": ERROR: Failed to open input file \""
             << inputFile << "\": " << sf_strerror(sndfile) << endl;
        return 1;
    }

    ofstream *out = 0;

    adaptSafe(&plugin);
    if (!plugin) {
        cerr << myname << ": ERROR: Failed to load plugin \"" << plugin->getIdentifier() << endl;
        sf_close(sndfile);
        if (out) {
            out->close();
            delete out;
        }
        return 1;
    }

    cerr << "Running plugin: \"" << plugin->getIdentifier() << "\"..." << endl;

    size_t blockSize = plugin->getPreferredBlockSize();
    size_t stepSize = plugin->getPreferredStepSize();

    if (blockSize == 0) {
        blockSize = 1024;
    }
    if (stepSize == 0) {
        if (plugin->getInputDomain() == Plugin::FrequencyDomain) {
            stepSize = blockSize/2;
        } else {
            stepSize = blockSize;
        }
    } else if (stepSize > blockSize) {
        cerr << "WARNING: stepSize " << stepSize << " > blockSize " << blockSize << ", resetting blockSize to ";
        if (plugin->getInputDomain() == Plugin::FrequencyDomain) {
            blockSize = stepSize * 2;
        } else {
            blockSize = stepSize;
        }
        cerr << blockSize << endl;
    }
    size_t overlapSize = blockSize - stepSize;
    sf_count_t currentStep = 0;
    int finalStepsRemaining = max(1, (static_cast<int>(blockSize) / static_cast<int>(stepSize)) - 1); // at end of file, this many part-silent frames needed after we hit EOF

    int channels = sfinfo.channels;

    float *filebuf = new float[blockSize * channels];
    float **plugbuf = new float*[channels];
    for (int c = 0; c < channels; ++c) plugbuf[c] = new float[blockSize + 2];

    cerr << "Using block size = " << blockSize << ", step size = "
              << stepSize << endl;

    // The channel queries here are for informational purposes only --
    // a PluginChannelAdapter is being used automatically behind the
    // scenes, and it will take case of any channel mismatch

    size_t minch = plugin->getMinChannelCount();
    size_t maxch = plugin->getMaxChannelCount();
    cerr << "Plugin accepts " << minch << " -> " << maxch << " channel(s)" << endl;
    cerr << "Sound file has " << channels << " (will mix/augment if necessary)" << endl;

    Plugin::OutputList outputs = plugin->getOutputDescriptors();
    Plugin::OutputDescriptor od;

    int returnValue = 1;
    int progress = 0;

    RealTime rt;
    PluginWrapper *wrapper = 0;
    RealTime adjustment = RealTime::zeroTime;

    if (outputs.empty()) {
        cerr << "ERROR: Plugin has no outputs!" << endl;
        goto done;
    }

    if (outputNo < 0) {

        for (size_t oi = 0; oi < outputs.size(); ++oi) {
            if (outputs[oi].identifier == output) {
                outputNo = oi;
                break;
            }
        }

        if (outputNo < 0) {
            cerr << "ERROR: Non-existent output \"" << output << "\" requested" << endl;
            goto done;
        }

    } else {

        if (int(outputs.size()) <= outputNo) {
            cerr << "ERROR: Output " << outputNo << " requested, but plugin has only " << outputs.size() << " output(s)" << endl;
            goto done;
        }        
    }

    od = outputs[outputNo];
    cerr << "Output is: \"" << od.identifier << "\"" << endl;

    if (!plugin->initialise(channels, stepSize, blockSize)) {
        cerr << "ERROR: Plugin initialise (channels = " << channels
             << ", stepSize = " << stepSize << ", blockSize = "
             << blockSize << ") failed." << endl;
        goto done;
    }

    wrapper = dynamic_cast<PluginWrapper *>(plugin);
    if (wrapper) {
        // See documentation for
        // PluginInputDomainAdapter::getTimestampAdjustment
        PluginInputDomainAdapter *ida =
            wrapper->getWrapper<PluginInputDomainAdapter>();
        if (ida) adjustment = ida->getTimestampAdjustment();
    }
    
    // Here we iterate over the frames, avoiding asking the numframes in case it's streaming input.
    do {

        int count;

        if ((blockSize==stepSize) || (currentStep==0)) {
            // read a full fresh block
            if ((count = sf_readf_float(sndfile, filebuf, blockSize)) < 0) {
                cerr << "ERROR: sf_readf_float failed: " << sf_strerror(sndfile) << endl;
                break;
            }
            if (count != blockSize) --finalStepsRemaining;
        } else {
            //  otherwise shunt the existing data down and read the remainder.
            memmove(filebuf, filebuf + (stepSize * channels), overlapSize * channels * sizeof(float));
            if ((count = sf_readf_float(sndfile, filebuf + (overlapSize * channels), stepSize)) < 0) {
                cerr << "ERROR: sf_readf_float failed: " << sf_strerror(sndfile) << endl;
                break;
            }
            if (count != stepSize) --finalStepsRemaining;
            count += overlapSize;
        }

        for (int c = 0; c < channels; ++c) {
            int j = 0;
            while (j < count) {
                plugbuf[c][j] = filebuf[j * sfinfo.channels + c];
                ++j;
            }
            while (j < blockSize) {
                plugbuf[c][j] = 0.0f;
                ++j;
            }
        }

        rt = RealTime::frame2RealTime(currentStep * stepSize, sfinfo.samplerate);

        printFeatures
            (RealTime::realTime2Frame(rt + adjustment, sfinfo.samplerate),
             sfinfo.samplerate, outputNo, plugin->process(plugbuf, rt),
             out, useFrames);

        if (sfinfo.frames > 0){
            int pp = progress;
            progress = (int)((float(currentStep * stepSize) / sfinfo.frames) * 100.f + 0.5f);
            if (progress != pp && out) {
                cerr << "\r" << progress << "%";
            }
        }

        ++currentStep;

    } while (finalStepsRemaining > 0);

    if (out) cerr << "\rDone" << endl;

    rt = RealTime::frame2RealTime(currentStep * stepSize, sfinfo.samplerate);

    printFeatures(RealTime::realTime2Frame(rt + adjustment, sfinfo.samplerate),
                  sfinfo.samplerate, outputNo,
                  plugin->getRemainingFeatures(), out, useFrames);

    returnValue = 0;

done:
    delete plugin;
    if (out) {
        out->close();
        delete out;
    }
    sf_close(sndfile);
    return returnValue;
}

void
printFeatures(int frame, int sr, int output,
              Plugin::FeatureSet features, ofstream *out, bool useFrames)
{
    for (unsigned int i = 0; i < features[output].size(); ++i) {

        if (useFrames) {

            int displayFrame = frame;

            if (features[output][i].hasTimestamp) {
                displayFrame = RealTime::realTime2Frame
                    (features[output][i].timestamp, sr);
            }

            (out ? *out : cout) << displayFrame;

            if (features[output][i].hasDuration) {
                displayFrame = RealTime::realTime2Frame
                    (features[output][i].duration, sr);
                (out ? *out : cout) << "," << displayFrame;
            }

            (out ? *out : cout)  << ":";

        } else {

            RealTime rt = RealTime::frame2RealTime(frame, sr);

            if (features[output][i].hasTimestamp) {
                rt = features[output][i].timestamp;
            }

            (out ? *out : cout) << rt.toString();

            if (features[output][i].hasDuration) {
                rt = features[output][i].duration;
                (out ? *out : cout) << "," << rt.toString();
            }

            (out ? *out : cout) << ":";
        }

        for (unsigned int j = 0; j < features[output][i].values.size(); ++j) {
            (out ? *out : cout) << " " << features[output][i].values[j];
        }
        (out ? *out : cout) << " " << features[output][i].label;
        std::string v = features[output][i].label;
        (out ? *out : cout) << endl;
    }
}
