#!/bin/bash
echo "SCRIPT: PROJECT CLEANUP"
echo "(may need root privilages)"
echo "| DESCRIPTION: Resets the project directory structure to the"
echo "| way it was before anything was done. Removes unnecessary "
echo "| directories and files to make sure project is reset to"
echo "| the original state."
echo ""
read -p "Want to use the script? [yes/no]: " choice;
echo ""

if [[ "$choice" == "yes" ]]; then
    echo "STATUS: Doing project cleanup."
    sudo rm -r cmake-build-debug processed_data bin .idea &> /dev/null
else
    echo "STATUS: You chose not to use the script."
fi


