//
//  TestUtilities.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 31/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "TestUtilities.h"
#include <cmath>
#include <numeric>
#include <algorithm>

std::vector<float> TestUtilities::generateSineWave(float frequency, float lengthSeconds, float sampleRate, float amplitude)
{
    // this is really inefficient (only need to generate one period), but it doens't matter for testing purposes
    const double angularFrequency = 2.0 * std::acos(-1) * frequency;
    std::vector<float> sineWave(static_cast<int>(sampleRate * lengthSeconds));
    std::iota(sineWave.begin(), sineWave.end(), 0.0);
    std::transform(sineWave.begin(), sineWave.end(), sineWave.begin(), [&](const float& n) {
        return amplitude * std::sin( angularFrequency / sampleRate * n);
    });
    return sineWave;
}
