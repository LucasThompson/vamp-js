//
//  SndFileAudioStream.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 19/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "SndFileAudioStream.h"

SndFileAudioStream::SndFileAudioStream(std::string filePath)
{
    memset(&mInfo, 0, sizeof(SF_INFO));
    mFile = sf_open(filePath.c_str(), SFM_READ, &mInfo);
}

void SndFileAudioStream::initStreamBuffer(int blockSize)
{
    mFileBuffer = std::unique_ptr<float>{new float[blockSize * getChannelCount()]};
}

SndFileAudioStream::~SndFileAudioStream()
{
    sf_close(mFile);
}

int SndFileAudioStream::getChannelCount()
{
    return mInfo.channels;
}

int SndFileAudioStream::readBlock(int blockSize, int readPointer)
{
    if (readPointer > 0) {
        memmove(mFileBuffer.get(), mFileBuffer.get() + (blockSize * getChannelCount()), readPointer * getChannelCount() * sizeof(float));
    }
    int count = static_cast<int>(sf_readf_float(mFile, mFileBuffer.get() + (readPointer * getChannelCount()), blockSize));
    if (count < blockSize) {
        std::fill(mFileBuffer.get() + count + (readPointer * getChannelCount()),
                  mFileBuffer.get() + (blockSize - count) * getChannelCount(),
                  0.0f);
    }
    return count;
}

float SndFileAudioStream::getSample(int n, int channel)
{
    return mFileBuffer.get()[n * getChannelCount() + channel];
}

void SndFileAudioStream::setSample(int n, int channel, float sample)
{
    mFileBuffer.get()[n * getChannelCount() + channel] = sample;
}

float SndFileAudioStream::getSampleRate()
{
    return mInfo.samplerate;
}