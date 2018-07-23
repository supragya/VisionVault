#!/bin/bash
echo "SCRIPT: BUILD AND SETUP SYSTEMS"
echo "(may need root privilages)"
echo "| DESCRIPTION: Builds the generation and emulation systems"
echo "| in the appropriate directories. Advised using clean_project.sh"
echo "| along with this script to clean project folder before compilation"
echo ""
read -p "Want to use the script? [yes/no]: " choice;
echo ""

if [[ "$choice" == "yes" ]]; then
    echo "STATUS: Building systems."
    echo "Setting up the directories (ignore errors)..."
    mkdir bin build
    echo "Running cmake on the source"
    cd build
    cmake ../
    make
    cd ..
    echo "Extracting binaries..."
    cp build/src/generator/Generator bin/
    cp build/src/stream_handler/StreamHandler bin/
    echo "Removing build directory..."
    sudo rm -r build/
    echo "STATUS: Compiled and setup successfully"
else
    echo "STATUS: You chose not to use the script."
fi


