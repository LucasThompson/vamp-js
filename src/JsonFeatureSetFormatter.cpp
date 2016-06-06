//
//  JsonFeatureSetFormatter.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 06/06/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "JsonFeatureSetFormatter.h"
#include "json11.hpp"

using json11::Json;

JsonFeatureSetFormatter::JsonFeatureSetFormatter() : mOutputNumber(0)
{}

JsonFeatureSetFormatter::JsonFeatureSetFormatter(int outputNumber) : mOutputNumber(outputNumber)
{}

std::string JsonFeatureSetFormatter::formatFeature(long frame, Vamp::Plugin::FeatureSet features, float sampleRate)
{
    Vamp::RealTime rt = Vamp::RealTime::frame2RealTime(frame, sampleRate);
    
    for (int i = 0; i < features[mOutputNumber].size(); ++i) {
        if (features[mOutputNumber][i].hasTimestamp) {
            rt = features[mOutputNumber][i].timestamp;
        }
        
        mTimes.push_back(std::stod(rt.toString()));
        std::vector<float> values;
        
        for (int j = 0; j < features[mOutputNumber][i].values.size(); ++j) {
            values.push_back(features[mOutputNumber][i].values[j]);
        }
        mValues.push_back(values);
    }
    return "";
}

std::string JsonFeatureSetFormatter::formatRemainingFeatures(long frame, Vamp::Plugin::FeatureSet features, float sampleRate)
{
    formatFeature(frame, features, sampleRate);
    Json json = Json::object {
        { "feature", Json::array
            {
                Json::object
                {
                    {"data", Json::array
                        {
                            Json::object
                            {
                                {"value", mValues },
                                {"time", mTimes}
                            }
                        }
                    }}
            }}
    };
    return json.dump();
}