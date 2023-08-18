#!/bin/bash

# Build the C++ project using CMake and make
mkdir -p /app/build
cd /app/build
cmake ..
make

# Run the C++ project
cd ..
./build/StreamChannelAssembler $out