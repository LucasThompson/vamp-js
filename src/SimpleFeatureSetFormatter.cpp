//
//  SimpleFeatureSetFormatter.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 20/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "SimpleFeatureSetFormatter.h"
#include <sstream>

SimpleFeatureSetFormatter::SimpleFeatureSetFormatter() : mOutputNumber(0)
{}

SimpleFeatureSetFormatter::SimpleFeatureSetFormatter(int outputNumber) : mOutputNumber(outputNumber)
{}

std::string SimpleFeatureSetFormatter::formatFeature(long frame, const Vamp::Plugin::FeatureSet& features, float sampleRate)
{
    if (features.count(mOutputNumber) == 0) return {};
    std::stringstream output;

    Vamp::RealTime rt = Vamp::RealTime::frame2RealTime(frame, sampleRate);
    

    for (int i = 0; i < features.at(mOutputNumber).size(); ++i)
    {
        if (features.at(mOutputNumber)[i].hasTimestamp) {
            rt = features.at(mOutputNumber)[i].timestamp;
        }
    
        output << rt.toString();
    
        if (features.at(mOutputNumber)[i].hasDuration) {
            rt = features.at(mOutputNumber)[i].duration;
            output << "," << rt.toString();
        }
    
        output << ":";
    
        for (int j = 0; j < features.at(mOutputNumber)[i].values.size(); ++j) {
            output << " " << features.at(mOutputNumber)[i].values[j];
        }
        output << std::endl;
    }
    return output.str();
}

std::string SimpleFeatureSetFormatter::formatRemainingFeatures(long frame, const Vamp::Plugin::FeatureSet& features, float sampleRate)
{
    return formatFeature(frame, features, sampleRate);
}