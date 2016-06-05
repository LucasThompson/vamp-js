//
//  VampJsBindings.h
//  Vamp.js
//
//  Created by Lucas Thompson on 03/06/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#ifndef host_bindings_h
#define host_bindings_h

#include <emscripten/bind.h>
#include "../src/AudioStream.h"
#include "../src/RawDataAudioStream.h"
#include "../src/FeatureSetFormatter.h"
#include "../src/SimpleFeatureSetFormatter.h"
#include "../src/VampHost.h"
#include "./plugins/ZeroCrossing.h"
#include "./plugins/PowerSpectrum.h"
#include "./plugins/FixedTempoEstimator.h"
#include "./plugins/pyin/PYinVamp.h"

template<class Base, class Derived, typename... Targs>
std::unique_ptr<Base> createUniquePtr(Targs... Fargs)
{
    return std::unique_ptr<Base>{new Derived(Fargs...)};
}

std::unique_ptr<VampHost> createHost(std::unique_ptr<Vamp::Plugin> plugin)
{
    return std::unique_ptr<VampHost>{new VampHost{std::move(plugin)}};
}

EMSCRIPTEN_BINDINGS(vamp_js)
{
    emscripten::class_<AudioStream>("AudioStream")
        .function("setSample", &AudioStream::setSample);
    emscripten::class_<RawDataAudioStream, emscripten::base<AudioStream>>("RawDataAudioStream")
        .constructor<int, float, float>();
    emscripten::class_<FeatureSetFormatter>("FeatureSetFormatter")
        .function("format", &FeatureSetFormatter::format);
    emscripten::class_<SimpleFeatureSetFormatter, emscripten::base<FeatureSetFormatter>>("SimpleFeatureSetFormatter")
        .constructor<>();
    emscripten::class_<VampHost>("VampHost")
        .constructor<>(&createHost)
        .function("run", &VampHost::run);
    emscripten::class_<Vamp::Plugin>("Plugin");
    emscripten::class_<ZeroCrossing, emscripten::base<Vamp::Plugin>>("ZeroCrossing")
        .constructor<float>();
    emscripten::class_<PowerSpectrum, emscripten::base<Vamp::Plugin>>("PowerSpectrum")
        .constructor<float>();
    emscripten::class_<FixedTempoEstimator, emscripten::base<Vamp::Plugin>>("FixedTempoEstimator")
        .constructor<float>();
    emscripten::class_<PYinVamp, emscripten::base<Vamp::Plugin>>("PYin")
        .constructor<float>();
    emscripten::function("createPYin", &createUniquePtr<Vamp::Plugin, PYinVamp, float>);
    emscripten::function("createZeroCrossing", &createUniquePtr<Vamp::Plugin, ZeroCrossing, float>);
    emscripten::function("createPowerSpectrum", &createUniquePtr<Vamp::Plugin, PowerSpectrum, float>);
    emscripten::function("createFixedTempoEstimator", &createUniquePtr<Vamp::Plugin, FixedTempoEstimator, float>);
    emscripten::function("createRawDataAudioStream", &createUniquePtr<AudioStream, RawDataAudioStream, int, float, float>);
    emscripten::function("createSimpleFeatureSetFormatter", &createUniquePtr<FeatureSetFormatter, SimpleFeatureSetFormatter, int>);
}

#endif /* host_bindings_h */
