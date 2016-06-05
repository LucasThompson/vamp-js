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
#include <memory>

class SndFileAudioStream : public AudioStream
{
public:
    SndFileAudioStream(std::string filePath);
    ~SndFileAudioStream();
    void initStreamBuffer(int blockSize) override;
    int getChannelCount() override;
    float getSampleRate() override;
    int readBlock(int blockSize, int readPointer) override;
    float getSample(int n, int channel) override;
    void setSample(int n, int channel, float sample) override;
private:
    SNDFILE* mFile;
    SF_INFO mInfo;
    std::unique_ptr<float> mFileBuffer;
};

#endif /* SndFileAudioStream_hpp */
