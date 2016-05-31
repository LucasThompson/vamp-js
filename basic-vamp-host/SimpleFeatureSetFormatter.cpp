//
//  SimpleFeatureSetFormatter.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 20/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "SimpleFeatureSetFormatter.h"
#include <sstream>

std::string SimpleFeatureSetFormatter::format(long frame, Vamp::Plugin::FeatureSet features, float sampleRate)
{
    std::stringstream output;
    
    Vamp::RealTime rt = Vamp::RealTime::frame2RealTime(frame, sampleRate);
    
    for (int i = 0; i < features[0].size(); ++i)
    {
        if (features[0][i].hasTimestamp) {
            rt = features[0][i].timestamp;
        }
        
        output << rt.toString();
        
        if (features[0][i].hasDuration) {
            rt = features[0][i].duration;
            output << "," << rt.toString();
        }
        
        output << ":";
        
        for (int j = 0; j < features[0][i].values.size(); ++j) {
            output << " " << features[0][i].values[j];
        }
        output << std::endl;
    }
    return output.str();
}