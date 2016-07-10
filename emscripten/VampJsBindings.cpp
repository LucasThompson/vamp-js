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
#include "../src/JsonFeatureSetFormatter.h"
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
    emscripten::register_vector<std::string>("VectorString");
    emscripten::register_vector<Vamp::Plugin::OutputDescriptor>("VectorOutputDescriptor");
    emscripten::enum_<Vamp::Plugin::OutputDescriptor::SampleType>("SampleType")
        .value("OneSamplePerStep", Vamp::Plugin::OutputDescriptor::SampleType::OneSamplePerStep)
        .value("FixedSampleRate", Vamp::Plugin::OutputDescriptor::SampleType::FixedSampleRate)
        .value("VariableSampleRate", Vamp::Plugin::OutputDescriptor::SampleType::VariableSampleRate)
        ;

    emscripten::value_object<Vamp::Plugin::OutputDescriptor>("OutputDescriptor")
        .field("identifier", &Vamp::Plugin::OutputDescriptor::identifier)
        .field("name", &Vamp::Plugin::OutputDescriptor::name)
        .field("description", &Vamp::Plugin::OutputDescriptor::description)
        .field("unit", &Vamp::Plugin::OutputDescriptor::unit)
        .field("hasFixedBinCount", &Vamp::Plugin::OutputDescriptor::hasFixedBinCount)
        .field("binCount", &Vamp::Plugin::OutputDescriptor::binCount)
        .field("binNames", &Vamp::Plugin::OutputDescriptor::binNames)
        .field("hasKnownExtents", &Vamp::Plugin::OutputDescriptor::hasKnownExtents)
        .field("minValue", &Vamp::Plugin::OutputDescriptor::minValue)
        .field("maxValue", &Vamp::Plugin::OutputDescriptor::maxValue)
        .field("isQuantized", &Vamp::Plugin::OutputDescriptor::isQuantized)
        .field("quantizeStep", &Vamp::Plugin::OutputDescriptor::quantizeStep)
        .field("sampleType", &Vamp::Plugin::OutputDescriptor::sampleType)
        .field("sampleRate", &Vamp::Plugin::OutputDescriptor::sampleRate)
        .field("hasDuration", &Vamp::Plugin::OutputDescriptor::hasDuration)
        ;
    
    emscripten::class_<AudioStream>("AudioStream")
        .function("setSample", &AudioStream::setSample);
    emscripten::class_<RawDataAudioStream, emscripten::base<AudioStream>>("RawDataAudioStream")
        .constructor<int, float, float>();
    emscripten::class_<FeatureSetFormatter>("FeatureSetFormatter")
        .function("formatFeature", &FeatureSetFormatter::formatFeature)
        .function("formatRemainingFeatures", &FeatureSetFormatter::formatRemainingFeatures);
    emscripten::class_<SimpleFeatureSetFormatter, emscripten::base<FeatureSetFormatter>>("SimpleFeatureSetFormatter")
        .constructor<>();
    emscripten::class_<JsonFeatureSetFormatter, emscripten::base<FeatureSetFormatter>>("JsonFeatureSetFormatter")
        .constructor<>();
    emscripten::class_<VampHost>("VampHost")
        .constructor<>(&createHost)
        .function("run", &VampHost::run);
    emscripten::class_<Vamp::Plugin>("Plugin")
        .function("getOutputDescriptors", &Vamp::Plugin::getOutputDescriptors);
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
    emscripten::function("createJsonFeatureSetFormatter", &createUniquePtr<FeatureSetFormatter, JsonFeatureSetFormatter, int>);
}

#endif /* host_bindings_h */
