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
    SimpleFeatureSetFormatter();
    SimpleFeatureSetFormatter(int outputNumber);
    std::string formatFeature(long frame, const Vamp::Plugin::FeatureSet& features, float sampleRate) override;
    std::string formatRemainingFeatures(long frame, const Vamp::Plugin::FeatureSet& features, float sampleRate) override;
private:
    int mOutputNumber;
};

#endif /* SimpleFeatureSetFormatter_hpp */
