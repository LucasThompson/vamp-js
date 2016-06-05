//
//  testIntegrationSndAudioStream.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 06/06/2016.
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
#include "SimpleFeatureSetFormatter.h"
#include "SndFileAudioStream.h"

TEST_CASE("Valid output for simple sine wave, single channel, sndfile.")
{
    std::unique_ptr<AudioStream> stream
    {
        new SndFileAudioStream{"/Users/lucas/code/dsp/basic-vamp-host/TestVampHost/fixtures/test.wav"}
    };
    SECTION("Zero crossings, sndfile")
    {
        VampHost host{std::unique_ptr<Vamp::Plugin>{new ZeroCrossing{stream->getSampleRate()}}};
        REQUIRE( TestFixture::expected_zc_440 == host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter()}));
    }
    SECTION("Power Spectrum, sndfile")
    {
        VampHost host{std::unique_ptr<Vamp::Plugin>{new PowerSpectrum{stream->getSampleRate()}}};
        REQUIRE( TestFixture::expected_ps_440 == host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter()}));
    }
    SECTION("Tempo Detection Function, sndfile")
    {
        VampHost host{std::unique_ptr<Vamp::Plugin>{new FixedTempoEstimator{stream->getSampleRate()}}};
        REQUIRE( TestFixture::expected_tempo_df_440 == host.run(std::move(stream), std::unique_ptr<SimpleFeatureSetFormatter>{new SimpleFeatureSetFormatter{2}}));
    }
}
