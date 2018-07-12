#!/usr/bin/env bash

echo "--------------------------------------------------------"
echo "AXIOM Raw Stream Handler - File Integrity emulation"
echo "Please run this script by cd(ing) into the project folder"
echo "--------------------------------------------------------"
echo "- requires root access"

echo "=== Beginning building binaries for emulation"
echo "1. Building cam2mlv"

sudo rm -r bin/
sudo rm -r build/
mkdir build
cd build
cmake ../
make
cd ..
mkdir bin
cp build/src/generator/Generator bin/Generator
cp build/src/stream_handler/StreamHandler bin/StreamHandler
#sudo rm -r build/
cd bin

echo "2. Building mlv2dng"
cd ../mlvfs
./build_installer.sh

echo "=== Write cam stream"
cd ../bin
./Generator

echo "=== Running cam2mlv"
./StreamHandler

echo "=== Test MLV file - MLVtestresult.txt"
cd ..
./mlv_dump.linux -v bin/axiom.mlv --dng

echo "=== Running mlv2dng"
cd ../mlvfs/mlvfs
sudo umount /mnt
sudo ./mlvfs /mnt --mlv_dir=/home/supragya/AXIOM_RawStreamHandler/bin/
