//
//  testMap.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 22/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "catch.hpp"
#include <algorithm>


TEST_CASE("Use std::transform instead of raw loop")
{
    int channels = 2;
    int blockSize = 10;
    float** pluginBuffer = new float*[channels];
    for (int c = 0; c < channels; ++c) pluginBuffer[c] = new float[blockSize + 2];
    int count = 8;
    SECTION("Old Way")
    {
        for (int c = 0; c < channels; ++c) {
            int j = 0;
            while (j < count) {
                pluginBuffer[c][j] = 1.0f;
                ++j;
            }
            while (j < blockSize) {
                pluginBuffer[c][j] = 0.0f;
                ++j;
            }
        }
        REQUIRE(pluginBuffer[0][7] == 1.0f);
        REQUIRE(pluginBuffer[0][8] == 0.0f);
    }
    
    SECTION("New Way")
    {
        for (int c = 0; c < channels; ++c)
        {
            std::transform(pluginBuffer[c], pluginBuffer[c] + count, pluginBuffer[c], [](const float&) { return 1.0f; });
            std::transform(pluginBuffer[c] + count, pluginBuffer[c] + blockSize, pluginBuffer[c] + count, [](const float&) { return 0.0f; });
        }
        REQUIRE(pluginBuffer[0][7] == 1.0f);
        REQUIRE(pluginBuffer[0][8] == 0.0f);
        
//        [&](const float& x) {
//            size_t index = &x - &pluginBuffer[c][0];
//            return 2.0f;
//        }
    }
}
