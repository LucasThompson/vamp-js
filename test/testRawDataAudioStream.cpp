//
//  testRawDataAudioStream.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 02/06/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "catch.hpp"
#include "RawDataAudioStream.h"
#include "TestUtilities.h"

TEST_CASE("Read valid samples")
{
    RawDataAudioStream stream
    {
        std::vector<std::vector<float>>
        {{
            {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}
        }},
        1.0
    };
    int blockSize = 4;
    stream.initStreamBuffer(blockSize);
    
    SECTION("Hop size == block size")
    {
        REQUIRE(stream.readBlock(blockSize, 0) == blockSize);
        REQUIRE(stream.getSample(0, 0) == 1.0);
        REQUIRE(stream.getSample(1, 0) == 2.0);
        REQUIRE(stream.getSample(2, 0) == 3.0);
        REQUIRE(stream.getSample(3, 0) == 4.0);
        REQUIRE(stream.readBlock(blockSize, 0) == blockSize);
        REQUIRE(stream.getSample(0, 0) == 5.0);
        REQUIRE(stream.getSample(1, 0) == 6.0);
        REQUIRE(stream.getSample(2, 0) == 7.0);
        REQUIRE(stream.getSample(3, 0) == 8.0);
        REQUIRE(stream.readBlock(blockSize, 0) == blockSize - 1);
        REQUIRE(stream.getSample(0, 0) == 9.0);
        REQUIRE(stream.getSample(1, 0) == 10.0);
        REQUIRE(stream.getSample(2, 0) == 11.0);
        REQUIRE(stream.getSample(3, 0) == 0.0);
    }
    
    SECTION("Hop size == 0.5 * block size")
    {
        int hopSize = blockSize * 0.5;
        int overlap = blockSize - hopSize;
        REQUIRE(stream.readBlock(blockSize, 0) == blockSize);
        REQUIRE(stream.getSample(0, 0) == 1.0);
        REQUIRE(stream.getSample(1, 0) == 2.0);
        REQUIRE(stream.getSample(2, 0) == 3.0);
        REQUIRE(stream.getSample(3, 0) == 4.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 3.0);
        REQUIRE(stream.getSample(1, 0) == 4.0);
        REQUIRE(stream.getSample(2, 0) == 5.0);
        REQUIRE(stream.getSample(3, 0) == 6.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 5.0);
        REQUIRE(stream.getSample(1, 0) == 6.0);
        REQUIRE(stream.getSample(2, 0) == 7.0);
        REQUIRE(stream.getSample(3, 0) == 8.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 7.0);
        REQUIRE(stream.getSample(1, 0) == 8.0);
        REQUIRE(stream.getSample(2, 0) == 9.0);
        REQUIRE(stream.getSample(3, 0) == 10.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize - 1);
        REQUIRE(stream.getSample(0, 0) == 9.0);
        REQUIRE(stream.getSample(1, 0) == 10.0);
        REQUIRE(stream.getSample(2, 0) == 11.0);
        REQUIRE(stream.getSample(3, 0) == 0.0);
    }
    
    SECTION("Hop size == 0.25 * block size")
    {
        int hopSize = blockSize * 0.25;
        int overlap = blockSize - hopSize;
        REQUIRE(stream.readBlock(blockSize, 0) == blockSize);
        REQUIRE(stream.getSample(0, 0) == 1.0);
        REQUIRE(stream.getSample(1, 0) == 2.0);
        REQUIRE(stream.getSample(2, 0) == 3.0);
        REQUIRE(stream.getSample(3, 0) == 4.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 2.0);
        REQUIRE(stream.getSample(1, 0) == 3.0);
        REQUIRE(stream.getSample(2, 0) == 4.0);
        REQUIRE(stream.getSample(3, 0) == 5.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 3.0);
        REQUIRE(stream.getSample(1, 0) == 4.0);
        REQUIRE(stream.getSample(2, 0) == 5.0);
        REQUIRE(stream.getSample(3, 0) == 6.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 4.0);
        REQUIRE(stream.getSample(1, 0) == 5.0);
        REQUIRE(stream.getSample(2, 0) == 6.0);
        REQUIRE(stream.getSample(3, 0) == 7.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 5.0);
        REQUIRE(stream.getSample(1, 0) == 6.0);
        REQUIRE(stream.getSample(2, 0) == 7.0);
        REQUIRE(stream.getSample(3, 0) == 8.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 6.0);
        REQUIRE(stream.getSample(1, 0) == 7.0);
        REQUIRE(stream.getSample(2, 0) == 8.0);
        REQUIRE(stream.getSample(3, 0) == 9.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 7.0);
        REQUIRE(stream.getSample(1, 0) == 8.0);
        REQUIRE(stream.getSample(2, 0) == 9.0);
        REQUIRE(stream.getSample(3, 0) == 10.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize);
        REQUIRE(stream.getSample(0, 0) == 8.0);
        REQUIRE(stream.getSample(1, 0) == 9.0);
        REQUIRE(stream.getSample(2, 0) == 10.0);
        REQUIRE(stream.getSample(3, 0) == 11.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == hopSize - 1);
        REQUIRE(stream.getSample(0, 0) == 9.0);
        REQUIRE(stream.getSample(1, 0) == 10.0);
        REQUIRE(stream.getSample(2, 0) == 11.0);
        REQUIRE(stream.getSample(3, 0) == 0.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == 0);
        REQUIRE(stream.getSample(0, 0) == 10.0);
        REQUIRE(stream.getSample(1, 0) == 11.0);
        REQUIRE(stream.getSample(2, 0) == 0.0);
        REQUIRE(stream.getSample(3, 0) == 0.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == 0);
        REQUIRE(stream.getSample(0, 0) == 11.0);
        REQUIRE(stream.getSample(1, 0) == 0.0);
        REQUIRE(stream.getSample(2, 0) == 0.0);
        REQUIRE(stream.getSample(3, 0) == 0.0);
        REQUIRE(stream.readBlock(hopSize, overlap) == 0);
        REQUIRE(stream.getSample(0, 0) == 0.0);
        REQUIRE(stream.getSample(1, 0) == 0.0);
        REQUIRE(stream.getSample(2, 0) == 0.0);
        REQUIRE(stream.getSample(3, 0) == 0.0);
    }
}

TEST_CASE("Test set value")
{
    RawDataAudioStream stream{11, 1, 1.0};
    stream.setSample(0, 0, 10.0f);
    REQUIRE( stream.getSample(0, 0) == 10.0f);
}
