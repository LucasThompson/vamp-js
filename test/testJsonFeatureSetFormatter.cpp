//
//  testJsonFeatureSetFormatter.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 06/06/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "catch.hpp"
#include "JsonFeatureSetFormatter.h"
#include "VampHost.h"
#include "RawDataAudioStream.h"

class StubVamp : public Vamp::Plugin
{
public:
    StubVamp(float sampleRate) : Vamp::Plugin(sampleRate) {}
    ~StubVamp() = default;
    FeatureSet process(const float *const *inputBuffers,
                       Vamp::RealTime timestamp) { return FeatureSet(); };
    
    FeatureSet getRemainingFeatures()
    {
        FeatureSet fs;
        Feature one;
        one.hasTimestamp = true;
        one.timestamp = Vamp::RealTime::fromSeconds(0.123);
        one.values.push_back(1);
        one.values.push_back(2);
        one.values.push_back(3);
        Feature two;
        two.hasTimestamp = true;
        two.values.push_back(4);
        two.values.push_back(5);
        two.values.push_back(6);
        two.timestamp = Vamp::RealTime::fromSeconds(0.25);
        fs[0].push_back(one);
        fs[0].push_back(two);
        return fs;
    };
    
    bool initialise(size_t channels, size_t stepSize, size_t blockSize) { return true; };
    void reset() {};
    InputDomain getInputDomain() const { return TimeDomain; }
    std::string getIdentifier() const { return {}; }
    std::string getName() const { return {}; }
    std::string getDescription() const { return {}; }
    std::string getMaker() const { return {}; }
    int getPluginVersion() const { return 0; }
    std::string getCopyright() const { return {}; }
    OutputList getOutputDescriptors() const { return {}; }
};

TEST_CASE("Produces valid JSON")
{
    std::unique_ptr<AudioStream> stream
    {
        new RawDataAudioStream{0, 1, 1.0}
    };
    VampHost host{std::unique_ptr<Vamp::Plugin>{new StubVamp{stream->getSampleRate()}}};
    std::string expected{"{\"feature\": [{\"data\": [{\"time\": [0.123, 0.25], \"value\": [[1, 2, 3], [4, 5, 6]]}]}]}"};
    REQUIRE( expected == host.run(std::move(stream), std::unique_ptr<JsonFeatureSetFormatter>{new JsonFeatureSetFormatter}));
}