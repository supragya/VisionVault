#!/bin/bash
echo "SCRIPT: CHECK VERSIONS"
echo "| DESCRIPTION: Provides an overview of the versions that your"
echo "| system has of various dependencies vs the environment in which"
echo "| this system was built."
echo ""

echo "--------------------------------------------------------"
echo "CMAKE"
echo "Developed on: "
echo "cmake version 3.10.2"
echo "Your version: "
cmake --version
echo "--------------------------------------------------------"

echo "--------------------------------------------------------"
echo "GNU MAKE"
echo "Developed on: "
echo "GNU Make 4.1"
echo "Your version: "
make -v
echo "--------------------------------------------------------"

echo "--------------------------------------------------------"
echo "GCC"
echo "Developed on: "
echo "gcc version 7.3.0 (Ubuntu 7.3.0-16ubuntu3)"
echo "Your version: "
gcc -v
echo "--------------------------------------------------------"

echo "--------------------------------------------------------"
echo "DARKTABLE"
echo "Developed on: "
echo "darktable 2.4.2"
echo "Your version: "
darktable --version
echo "--------------------------------------------------------"