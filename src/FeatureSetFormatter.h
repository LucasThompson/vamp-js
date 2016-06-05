//
//  FeatureSetFormatter.h
//  basic-vamp-host
//
//  Created by Lucas Thompson on 19/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef FeatureSetFormatter_h
#define FeatureSetFormatter_h

#include <string>
#include <vamp-hostsdk/Plugin.h>

class FeatureSetFormatter
{
public:
    virtual ~FeatureSetFormatter() = default;
    virtual std::string format(long frame, Vamp::Plugin::FeatureSet features, float sampleRate) = 0;
};


#endif /* FeatureSetFormatter_h */
