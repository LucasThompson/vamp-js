# vamp.js

Build status (on Travis CI) [![Build Status](https://travis-ci.org/LucasThompson/vamp-js.svg?branch=master)](https://travis-ci.org/LucasThompson/vamp-js/)

## Info

This repo contains preliminary work on compiling Vamp Plugins to Javascript using Emscripten. 

There is a stripped down version of the simple vamp host from the vamp-hostsdk, with some refactoring to allow injecting different audio stream providers and output formatters, which can be called from Javascript along with a number of example plugins.

The approach taken was to expose C++ methods with embind. The use of C++11 smart pointers makes some of the calling code more awkward than it needs to be.

In this proof of concept implementation, the entire audio file is buffered into memory using the Web Audio API and the sample data is copied to an AudioStream which is passed to the Vamp Host, all from Javascript. Then the host's run method is called from Javascript to compute the features for the desired plugin. There is an output formatter which produces Javascript objects with a schema similar to JAMS. 

### Examples: 
* [Zero Crossing Counts](http://lucasthompson.github.io/vamp-js/examples/web-audio/zero-crossings.html) - This example uses the ZeroCrossing plugin from the Vamp Plugin SDK examples, and renders a graph using D3.js.
* [Spectrogram](http://lucasthompson.github.io/vamp-js/examples/web-audio/spectrogram.html) - This example uses the PowerSpectrum plugin from the Vamp Plugin SDK examples, and renders a simple black and white spectrogram using a HTML5 Canvas Element.

## Build

To build the C++ tests, you will need the meta build system cmake. 

Execute the build shell script in the root directory of the repo, this builds and runs the test suite:

```bash
./build.sh
```

To build the emscripten javascript module, you need em++ in your PATH. Then run make from the emscripten directory.

```bash
cd ./emscripten
make
```

## Notes

Performance is best in Firefox, due to its superior asm.js optimizations. It is worth noting that the Vamp plugins which do work in the frequency domain are quite slow. The Vamp SDK uses the publicly available Don Cross FFT implementation, and as noted in Chris Cannam's [blog post](https://thebreakfastpost.com/2015/10/18/ffts-in-javascript/) about compiling FFT libraries to Javascript, Kiss FFT is significantly faster. So I'll explore swapping that into the Vamp SDK at a later date.

I'd also like to explore implementing a host entirely in Javascript, using the Web Audio API node graph. 
