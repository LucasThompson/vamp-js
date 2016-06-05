//
//  RawDataAudioStream.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 31/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "RawDataAudioStream.h"

RawDataAudioStream::RawDataAudioStream(std::vector<std::vector<float>> data, float sampleRate) : mData(std::move(data)), mSampleRate(sampleRate), mCount(0), mBlockSize(0), mBlocks(0)
{
    mLength = static_cast<int>(mData[0].size());
}

RawDataAudioStream::RawDataAudioStream(int length, float channelCount, float sampleRate) : mSampleRate(sampleRate), mBlocks(0), mCount(0), mBlockSize(0), mLength(length)
{
    mData = std::vector<std::vector<float>>(channelCount);
    for (auto &buffer : mData) {
        buffer = std::vector<float>(length, 0.0f);
    }
}

void RawDataAudioStream::initStreamBuffer(int blockSize)
{
    mBlockSize = blockSize;
}

int RawDataAudioStream::getChannelCount()
{
    return static_cast<int>(mData.size());
}

float RawDataAudioStream::getSampleRate()
{
    return mSampleRate;
}

int RawDataAudioStream::readBlock(int blockSize, int readPointer)
{
    mBlockSize = blockSize;
    int diff = mLength - mCount;
    mCount += blockSize;
    mBlocks++;
    if (diff >= blockSize) {
        return blockSize;
    } else {
        return diff < 0 ? 0 : diff;
    }
}

float RawDataAudioStream::getSample(int n, int channel)
{
    int i = (mBlocks - 1) * mBlockSize + n;
    return i < mLength ? mData[channel][i] : 0.0;
}

void RawDataAudioStream::setSample(int n, int channel, float sample)
{
    // no bound checking for now
    mData[channel][n] = sample;
}

