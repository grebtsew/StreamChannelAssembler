#!/bin/bash

# Build the C++ project using CMake and make
mkdir -p /app/build
cd /app/build
cmake ..
make

# Run the C++ project
cd ..
echo " OUT COMMAND : $out"
./build/StreamChannelAssembler $out