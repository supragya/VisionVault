#!/bin/bash
echo "SCRIPT: APT - GET DEPENDENCIES"
echo "(may need root privilages)"
echo "| DESCRIPTION: Provides installation for necessary dependencies"
echo "| using apt-get install procedures. Works for debian based distros"
echo ""
read -p "Want to use the script? [yes/no]: " choice;
echo ""

if [[ "$choice" == "yes" ]]; then
    echo "STATUS: Doing project cleanup."
    sudo apt-get install build-essential libfuse-dev darktable cmake
else
    echo "STATUS: You chose not to use the script."
fi


