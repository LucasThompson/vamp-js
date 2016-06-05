//
//  TestUtilities.hpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 31/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef TestUtilities_hpp
#define TestUtilities_hpp

#include <vector>

class TestUtilities
{
public:
    static std::vector<float> generateSineWave(float frequency, float lengthSeconds, float sampleRate, float amplitude=1.0);
};

#endif /* TestUtilities_hpp */
