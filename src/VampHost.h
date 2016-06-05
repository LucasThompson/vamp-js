//
//  VampHost.hpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 16/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef VampHost_hpp
#define VampHost_hpp

#include <vamp-hostsdk/Plugin.h>
#include "AudioStream.h"
#include "FeatureSetFormatter.h"
#include <memory>

class VampHost
{
public:
    VampHost(std::unique_ptr<Vamp::Plugin> plugin);
    ~VampHost() = default;
    std::string run(std::unique_ptr<AudioStream> stream, std::unique_ptr<FeatureSetFormatter> formatter);
    
private:
    std::unique_ptr<Vamp::Plugin> mPlugin;
    size_t mBlockSize;
    size_t mStepSize;
    void initBufferSizes();
};

#endif /* VampHost_hpp */
