#!/usr/bin/env bash

sudo rm -r bin/
mkdir build
cd build
cmake ../
make
cd ..
mkdir bin
cp build/src/generator/Generator bin/Generator
cp build/src/stream_emulator/Framestream_emulator bin/Framestream_emulator
cp build/src/stream_emulator/Metastream_emulator bin/Metastream_emulator
cp build/src/stream_emulator/StreamHandler bin/StreamHandler
sudo rm -r build/