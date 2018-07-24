#!/bin/bash
echo "SCRIPT: RUN EMULATION"
echo "(may need root privilages)"
echo "| DESCRIPTION: Runs compiled binaries in correct order to "
echo "| emulate the end to end workflow from cam2mlv and mlv2dng"
echo "| Make sure you run \"build_all.sh\" before running this."
echo ""
read -p "Want to use the script? [yes/no]: " choice;
echo ""

if [[ "$choice" == "yes" ]]; then
    echo "STATUS: Running the systems."
    cd bin
    echo "RUNNING GENERATOR ---"
    ./Generator
    echo "RUNNING STREAMHANDLER ---"
    ./StreamHandler
    cd ..
    echo "RUNNING MLVDUMP ---"
    3rdParty/mlvdump/mlv_dump.linux processed_data/AXIOMBetaVideo.mlv --dng -v
    cd ..
else
    echo "STATUS: You chose not to use the script."
fi


