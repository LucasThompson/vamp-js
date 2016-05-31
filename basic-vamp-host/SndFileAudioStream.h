//
//  SndFileAudioStream.hpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 19/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef SndFileAudioStream_hpp
#define SndFileAudioStream_hpp

#include "AudioStream.h"
#include <string>
#include <sndfile.h>

class SndFileAudioStream : public AudioStream
{
public:
    SndFileAudioStream(std::string filePath);
    ~SndFileAudioStream();
    void initStreamBuffer(int blockSize) override;
    int getChannelCount() override;
    float getSampleRate() override;
    int readBlock(int blockSize, int readPointer) override;
    void shuntBlockBy(int readPointer, int nSamples) override;
    float getSample(int n, int channel) override;
private:
    SNDFILE* mFile;
    SF_INFO mInfo;
    float* mFileBuffer;
};

#endif /* SndFileAudioStream_hpp */
