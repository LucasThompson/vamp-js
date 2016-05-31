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
    mFileBuffer = new float[blockSize * getChannelCount()];
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
    return static_cast<int>(sf_readf_float(mFile, mFileBuffer + (readPointer * getChannelCount()), blockSize));
}

void SndFileAudioStream::shuntBlockBy(int readPointer, int nSamples)
{
    memmove(mFileBuffer, mFileBuffer + (readPointer * getChannelCount()), nSamples * getChannelCount() * sizeof(float));
}

float SndFileAudioStream::getSample(int n, int channel)
{
    return mFileBuffer[n * getChannelCount() + channel];
}

float SndFileAudioStream::getSampleRate()
{
    return mInfo.samplerate;
}