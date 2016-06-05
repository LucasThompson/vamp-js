//
//  RawDataAudioStream.hpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 31/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef RawDataAudioStream_hpp
#define RawDataAudioStream_hpp

#include "AudioStream.h"
#include <vector>

class RawDataAudioStream : public AudioStream
{
public:
    RawDataAudioStream(std::vector<std::vector<float>> data, float sampleRate);
    RawDataAudioStream(int length, float channelCount, float sampleRate);
    void initStreamBuffer(int blockSize) override;
    int getChannelCount() override;
    float getSampleRate() override;
    int readBlock(int blockSize, int readPointer) override;
    float getSample(int n, int channel) override;
    void setSample(int n, int channel, float sample) override;
private:
    std::vector<std::vector<float>> mData;
    float mSampleRate;
    int mCount;
    int mLength;
    int mBlockSize;
    int mBlocks;
};

#endif /* RawDataAudioStream_hpp */
