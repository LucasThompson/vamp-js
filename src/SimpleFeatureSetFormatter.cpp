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

std::string SimpleFeatureSetFormatter::formatFeature(long frame, Vamp::Plugin::FeatureSet features, float sampleRate)
{
    std::stringstream output;

    Vamp::RealTime rt = Vamp::RealTime::frame2RealTime(frame, sampleRate);

    for (int i = 0; i < features[mOutputNumber].size(); ++i)
    {
        if (features[mOutputNumber][i].hasTimestamp) {
            rt = features[mOutputNumber][i].timestamp;
        }
    
        output << rt.toString();
    
        if (features[mOutputNumber][i].hasDuration) {
            rt = features[mOutputNumber][i].duration;
            output << "," << rt.toString();
        }
    
        output << ":";
    
        for (int j = 0; j < features[mOutputNumber][i].values.size(); ++j) {
            output << " " << features[mOutputNumber][i].values[j];
        }
        output << std::endl;
    }
    return output.str();
}

std::string SimpleFeatureSetFormatter::formatRemainingFeatures(long frame, Vamp::Plugin::FeatureSet features, float sampleRate)
{
    return formatFeature(frame, features, sampleRate);
}