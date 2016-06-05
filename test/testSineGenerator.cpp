//
//  testSineGenerator.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 31/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "catch.hpp"
#include "fixtures/expected_sin.h"
#include "TestUtilities.h"


TEST_CASE("Test generator produces expected output from MATLAB")
{
    std::vector<float> actual_sin = TestUtilities::generateSineWave(440.0, 1.0, 8000.0, 0.5);
    REQUIRE(TestFixture::expected_sin == actual_sin);
}
