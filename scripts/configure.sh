#!/usr/bin/env bash

# Build project for Linux using the specified CMake Preset
# This script will build the specified preset/configuration from the command line.
# cmake and other dev tools/dependencies should be installed and configured in your distro/container

show_help() {
  printf "\nUsage: %s [options]\n" "$0"
    echo "Options:"
    echo "  --preset <name>        Specifies the build preset to use. Defaults to \"linux-clang\", "
    echo "                         see CMakePresets.json for the full list."

  exit 1
}

preset_name="linux-clang"

if [ $# -gt 0 ]; then
  if [[ $1 != --* ]]; then
    preset_name=$1
    shift
  fi
fi
while [ $# -gt 0 ]; do
  case "$1" in
    --preset)
      preset_name=$2
      shift
      ;;
    *)
      printf "\nUnknown argument: %s\n" "$1"
      show_help
      ;;
  esac
  shift
done

script_folder=$(dirname "$(readlink -f $0)")
repo_dir=$(dirname "$script_folder")
saved_location=$(pwd)
cd "$repo_dir" || exit

echo "Configuration preset $preset_name... using repo dir $repo_dir"

cmake --preset=$preset_name

cd "$saved_location"
