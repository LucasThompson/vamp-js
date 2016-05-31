//
//  SimpleFeatureSetFormatter.h
//  basic-vamp-host
//
//  Created by Lucas Thompson on 20/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef SimpleFeatureSetFormatter_hpp
#define SimpleFeatureSetFormatter_hpp

#include "FeatureSetFormatter.h"

class SimpleFeatureSetFormatter : public FeatureSetFormatter
{
public:
    SimpleFeatureSetFormatter() = default;
    std::string format(long frame, Vamp::Plugin::FeatureSet features, float sampleRate);
};

#endif /* SimpleFeatureSetFormatter_hpp */
