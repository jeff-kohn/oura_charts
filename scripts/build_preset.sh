#!/usr/bin/env bash

# Build project for Linux using the specified CMake Preset
# This script will build the specified preset/configuration from the command line.
# cmake and other dev tools/dependencies should be installed and configured in your distro/container

show_help() {
  printf "\nUsage: %s [options]\n" "$0"
    echo "Options:"
    echo "  --preset <name>        Specifies the build preset to use. Defaults to \"linux-x64-release\", "
    echo "                         see CMakePresets.json for the full list."
    echo "  --target <name>        Specifies the target to build (case-sensitive), defaults to \"all\","
    echo "                         can also be \"install\" or a specific project target."
    echo "  --skip-configure       If set, causes the 'configure' step to be skipped and just run "
    echo "                         the build command."
    echo "  --rebuild              If this switch is supplied, the --clean-first parametera will be passed"
  printf "                          to cmake to for a clean build.\n\n"
  exit 1
}

preset_name="linux-x64-release"
target="all"
skip_configure=false
rebuild=false

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
    --target)
      target=$2
      shift
      ;;
    --skip-configure)
      skip_configure=true
      ;;
    --rebuild)
      rebuild=true
      ;;
    --help)
      show_help
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

echo "Building totp_mfa linux for $preset_name... using repo dir $repo_dir"

if ! $skip_configure; then
   cmake --preset=$preset_name 
fi

if $rebuild; then
   cmake --build --preset=$preset_name --target=$target --clean-first
else
   cmake --build --preset=$preset_name --target=$target
fi

cd "$saved_location"
