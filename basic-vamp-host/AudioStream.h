//
//  AudioStream.h
//  basic-vamp-host
//
//  Created by Lucas Thompson on 19/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef AudioStream_h
#define AudioStream_h

class AudioStream
{
public:
    virtual ~AudioStream() = default;
    virtual void initStreamBuffer(int blockSize) = 0;
    virtual int getChannelCount() = 0;
    virtual float getSampleRate() = 0;
    virtual int readBlock(int blockSize, int readPointer=0) = 0;
    virtual void shuntBlockBy(int readPointer, int nSamples) = 0;
    virtual float getSample(int n, int channel) = 0;
};


#endif /* AudioStream_h */
