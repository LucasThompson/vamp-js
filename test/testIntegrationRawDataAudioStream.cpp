//
//  testIntegrationRawDataAudioStream.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 31/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "catch.hpp"
#include "fixtures/expected_zc.h"
#include "fixtures/expected_ps.h"
#include "fixtures/expected_tempo.h"
#include "VampHost.h"
#include "ZeroCrossing.h"
#include "PowerSpectrum.h"
#include "FixedTempoEstimator.h"
#include "RawDataAudioStream.h"
#include "SimpleFeatureSetFormatter.h"
#include "TestUtilities.h"


TEST_CASE("Valid output for simple sine wave, single channel, generated wave.")
{
    std::unique_ptr<AudioStream> stream
    {
        new RawDataAudioStream{std::vector<std::vector<float>>{{TestUtilities::generateSineWave(440.0, 1.0, 8000.0, 0.5)}}, 8000.0}
    };
    SECTION("Zero crossings, generated wave.")
    {
        VampHost host{std::unique_ptr<Vamp::Plugin>{new ZeroCrossing{stream->getSampleRate()}}};
        REQUIRE( TestFixture::expected_zc_440 == host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter()}));
    }
    SECTION("Power Spectrum, generated wave.")
    {
        VampHost host{std::unique_ptr<Vamp::Plugin>{new PowerSpectrum{stream->getSampleRate()}}};
        auto expected = TestFixture::expected_ps_440;
        const std::string actual = host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter()});
        REQUIRE( expected == actual );
    }
    SECTION("Tempo Detection Function, generated wave")
    {
        VampHost host{std::unique_ptr<Vamp::Plugin>{new FixedTempoEstimator{stream->getSampleRate()}}};
        REQUIRE( TestFixture::expected_tempo_df_440 == host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter{2}}));
    }
}
