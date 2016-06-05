cmake_minimum_required(VERSION 2.8)
project(VampJs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(src emscripten/ext emscripten/plugins emscripten/ext/src)
set(SOURCE_FILES src/RawDataAudioStream.cpp src/RawDataAudioStream.h src/SimpleFeatureSetFormatter.cpp src/SimpleFeatureSetFormatter.h src/VampHost.cpp src/VampHost.h)
set(TEST_UTILS test/TestUtilities.cpp test/TestUtilities.h)
set(TEST_FILES test/main.cpp test/catch.hpp test/testRawDataAudioStream.cpp test/testSineGenerator.cpp test/testIntegrationAudioStreams.cpp)
set(VAMP_SDK emscripten/ext/src/vamp-sdk/RealTime.cpp)
set(VAMP_HOST_SDK emscripten/ext/src/vamp-hostsdk/RealTime.cpp emscripten/ext/src/vamp-hostsdk/PluginChannelAdapter.cpp emscripten/ext/src/vamp-hostsdk/PluginInputDomainAdapter.cpp emscripten/ext/src/vamp-hostsdk/PluginWrapper.cpp)
set(VAMP_PLUGINS emscripten/plugins/ZeroCrossing.cpp emscripten/plugins/PowerSpectrum.cpp emscripten/plugins/FixedTempoEstimator.cpp)
add_executable(TestVampJs ${TEST_FILES} ${SOURCE_FILES} ${TEST_UTILS} ${VAMP_PLUGINS} ${VAMP_SDK} ${VAMP_HOST_SDK})

add_custom_command(TARGET TestVampJs COMMAND TestVampJs -r console -d yes -s)