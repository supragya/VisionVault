#!/usr/bin/env bash

mkdir build
cd build
cmake ../
make
cd ..
mkdir bin
cp build/src/generator/generator bin/generator
cp build/src/stream_emulator/Framestream_emulator bin/Framestream_emulator
cp build/src/stream_emulator/Metastream_emulator bin/Metastream_emulator
sudo rm -r build/