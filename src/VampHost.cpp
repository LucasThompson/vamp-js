//
//  VampHost.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 16/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "VampHost.h"
#include <vamp-hostsdk/PluginHostAdapter.h>
#include <vamp-hostsdk/PluginInputDomainAdapter.h>
#include <vamp-hostsdk/PluginChannelAdapter.h>
#include <algorithm>
#include <sstream>

using Vamp::HostExt::PluginChannelAdapter;
using Vamp::HostExt::PluginWrapper;
using Vamp::HostExt::PluginInputDomainAdapter;

VampHost::VampHost(std::unique_ptr<Vamp::Plugin> plugin) :
mPlugin(std::move(plugin))
{
    if (mPlugin->getInputDomain() == Vamp::Plugin::FrequencyDomain) {
        mPlugin = std::unique_ptr<Vamp::Plugin>{new PluginInputDomainAdapter{mPlugin.release()}};
    }
    mPlugin = std::unique_ptr<Vamp::Plugin>{new PluginChannelAdapter{mPlugin.release()}};
    initBufferSizes();
}

void VampHost::initBufferSizes()
{
    auto isFrequencyDomainPlugin = [&] { return mPlugin->getInputDomain() == Vamp::Plugin::FrequencyDomain; };
    auto calculateStepSizeIfZero = [&] { return isFrequencyDomainPlugin() ? mBlockSize * 0.5 : mBlockSize; };
    mBlockSize = mPlugin->getPreferredBlockSize() == 0 ? 1024 : mPlugin->getPreferredBlockSize();
    mStepSize = mPlugin->getPreferredStepSize() == 0 ? calculateStepSizeIfZero() : mPlugin->getPreferredStepSize();
    if (mStepSize > mBlockSize) mBlockSize = isFrequencyDomainPlugin() ? mStepSize * 2.0 : mStepSize;
}

std::string VampHost::run(std::unique_ptr<AudioStream> stream, std::unique_ptr<FeatureSetFormatter> formatter)
{
    std::stringstream output;
    
    const int channels = stream->getChannelCount();
    if (!mPlugin->initialise(channels, mStepSize, mBlockSize)) {
        return ""; // error
    }
    
    stream->initStreamBuffer(static_cast<int>(mBlockSize));
    const size_t overlapSize = mBlockSize - mStepSize;
    
    float** pluginBuffer = new float*[channels];
    for (int c = 0; c < channels; ++c) pluginBuffer[c] = new float[mBlockSize + 2];
    
    Vamp::RealTime adjustment = Vamp::RealTime::zeroTime;
    Vamp::RealTime rt;
    PluginWrapper* wrapper = dynamic_cast<PluginWrapper*>(mPlugin.get());
    
    if (wrapper) {
        PluginInputDomainAdapter* ida =
        wrapper->getWrapper<PluginInputDomainAdapter>();
        if (ida) adjustment = ida->getTimestampAdjustment();
    }
    
    long long currentStep = 0;
    int finalStepsRemaining = std::max(1, (static_cast<int>(mBlockSize) / static_cast<int>(mStepSize)) - 1); // at end of file, this many part-silent frames needed after we hit EOFs
    
    // Iterate over the frames, avoiding asking the numframes in case it's streaming input.
    do {
        int count = 0;
        
        if ((mBlockSize == mStepSize) || (currentStep == 0)) {
            if ((count = stream->readBlock(static_cast<int>(mBlockSize))) < 0) {
                return ""; // error
            }
            if (count != mBlockSize) --finalStepsRemaining;
        } else {
            if ((count = stream->readBlock(static_cast<int>(mStepSize),  static_cast<int>(overlapSize))) < 0) {
                return ""; // error
            }
            if (count != mStepSize) --finalStepsRemaining;
            count += overlapSize;
        }
        
        for (int c = 0; c < channels; ++c) {
            for (int n = 0; n < count; ++n) {
                pluginBuffer[c][n] = stream->getSample(n, c);
            }
            std::fill(pluginBuffer[c] + count, pluginBuffer[c] + mBlockSize, 0.0f);
        }
        
        rt = Vamp::RealTime::frame2RealTime(currentStep * mStepSize, stream->getSampleRate());
        long frame = Vamp::RealTime::realTime2Frame(rt + adjustment, stream->getSampleRate());
        
        output << formatter->formatFeature(frame, mPlugin->process(pluginBuffer, rt), stream->getSampleRate());
        ++currentStep;
        
    } while (finalStepsRemaining > 0);
    
    rt = Vamp::RealTime::frame2RealTime(currentStep * mStepSize, stream->getSampleRate());
    long frame = Vamp::RealTime::realTime2Frame(rt + adjustment, stream->getSampleRate());
    output << formatter->formatRemainingFeatures(frame, mPlugin->getRemainingFeatures(), stream->getSampleRate());
    return output.str();
}
