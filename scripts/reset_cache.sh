#!/usr/bin/env bash

script_folder=$(dirname "$(readlink -f $0)")
repo_dir=$(dirname "$script_folder")
build_dir="$repo_dir/builds"

printf  "\nThis script will delete the entire contents of the build directory\n\t%s\n" "$build_dir"
read -p "Press enter to continue, or CTRL+C to cancel:"

echo "Deleting files..."
rm -rf "$build_dir"
echo "...done."
echo -e "\nYou will need to run cmake configure before you can build.\n"