#!/usr/bin/env bash

echo "-----------------------------------------------------"
echo "AXIOM Beta Raw Video Container Format Full Conversion"
echo "-----------------------------------------------------"
echo "NOTICE: Emulation requires root access to continue."
echo "Use sudo su before running this emulation script."
echo "-----------------------------------------------------"


CURDIR = "/home/supragya/AXIOM_RawStreamHandler"

echo ">>>>>> Building cam2mlv binaries for emulation"
cd $CURDIR
sudo rm -r bin
mkdir build
cd build
cmake ../
make
cd ..
mkdir bin
cp build/src/generator/Generator bin/Generator
cp build/src/stream_handler/StreamHandler bin/StreamHandler
sudo rm -r build/

echo ">>>>>> Building mlv2dng (mlvfs) binaries for emulation"

cd $CURDIR
cd mlvfs
./build_installer.sh

echo ">>>>>> [cam] Running Generator"
cd $CURDIR/bin/
./Generator

echo ">>>>>> [cam2mlv] Running Stream Handler"
./StreamHandler

echo ">>>>>> Test MLV file"
./../mlv_dump/mlv_dump.linux -v axiom.mlv

echo ">>>>>> [mlv2dng] Running MLVFS"
mkdir /media/mlv
umount /media/mlv/
cd $CURDIR
./mlvfs/mlvfs/mlvfs $CURDIR/bin/ --mlv_dir=/media/mnt

echo ">>>>>> Retrieving results"
cd $CURDIR
rm -r dng_exports
mkdir dng_exports
cp /media/mlv/* dng_export/

echo ">>>>>> Unmounting /media/mlv/"
umount /media/mlv/

echo ">>>>>> Emulation ends here. If you did not encounter errors, DNG files can be found in dng_export folder"