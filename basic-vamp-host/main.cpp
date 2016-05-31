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
#include "SimpleFeatureSetFormatter.h"
#include "VampHost.h"



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
    

    std::unique_ptr<AudioStream> stream{new SndFileAudioStream{wavname}};
    VampHost host{std::unique_ptr<Vamp::Plugin>{new PowerSpectrum{stream->getSampleRate()}}};
    host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter()});
    return 0;
}