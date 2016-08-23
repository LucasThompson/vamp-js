#!/usr/bin/env bash
make -C ./emscripten/ SubModules
mkdir build
cd build
cmake ..
make
