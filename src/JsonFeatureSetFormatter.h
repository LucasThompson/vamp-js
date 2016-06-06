//
//  JsonFeatureSetFormatter.hpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 06/06/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef JsonFeatureSetFormatter_hpp
#define JsonFeatureSetFormatter_hpp

#include "FeatureSetFormatter.h"
#include <vector>
#include <string>

class JsonFeatureSetFormatter : public FeatureSetFormatter
{
public:
    JsonFeatureSetFormatter();
    JsonFeatureSetFormatter(int outputNumber);
    std::string formatFeature(long frame, Vamp::Plugin::FeatureSet features, float sampleRate) override;
    std::string formatRemainingFeatures(long frame, Vamp::Plugin::FeatureSet features, float sampleRate) override;
private:
    int mOutputNumber;
    std::vector<std::vector<float>> mValues;
    std::vector<double> mTimes;
};


#endif /* JsonFeatureSetFormatter_hpp */
