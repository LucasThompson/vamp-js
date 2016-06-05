//
//  main.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 13/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "ZeroCrossing.h"
#include "PowerSpectrum.h"
#include "SndFileAudioStream.h"
#include "RawDataAudioStream.h"
#include "TestUtilities.h"
#include "SimpleFeatureSetFormatter.h"
#include "VampHost.h"

class PowerSpectrum2048 : public PowerSpectrum
{
public:
    PowerSpectrum2048(float inputSampleRate) : PowerSpectrum(inputSampleRate) {}
    size_t getPreferredStepSize() const override { return 256; }
    size_t getPreferredBlockSize() const override { return 2048; }
};

int main(int argc, char **argv)
{
    char *scooter = argv[0];
    char *name = 0;
    while (scooter && *scooter) {
        if (*scooter == '/' || *scooter == '\\') name = ++scooter;
        else ++scooter;
    }
    if (!name || !*name) name = argv[0];

    
    bool useFrames = false;
    
    int base = 1;
    if (!strcmp(argv[1], "-s")) {
        useFrames = true;
        base = 2;
    }
    
    std::string wavname = argv[base + 1];
    

//    std::unique_ptr<AudioStream> stream
//    {
//        new RawDataAudioStream{std::vector<std::vector<float>>{{TestUtilities::generateSineWave(440.0, 1.0, 8000.0, 0.5)}}, 8000.0}
//    };
    std::unique_ptr<AudioStream> stream
    {
        new SndFileAudioStream{"/Users/lucas/SDKs/vamp-plugin-sdk-2.6/host/viper.wav"}
    };
    VampHost host{std::unique_ptr<Vamp::Plugin>{new PowerSpectrum{stream->getSampleRate()}}};
    std::cout << host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter()});
    return 0;
}