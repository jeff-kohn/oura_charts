#!/usr/bin/env bash

# Build project for Linux using the specified CMake Preset
# This script will build the specified preset/configuration from the command line.
# cmake and other dev tools/dependencies should be installed and configured in your distro/container

show_help() {
  printf "\nUsage: %s [options]\n" "$0"
    echo "Options:"
    echo "  --preset <name>        Specifies the build preset to use. Defaults to \"linux-clang\", "
    echo "                         see CMakePresets.json for the full list."
    echo "  --config <name>        Specifies the build configure to use. Defaults to \"Release\", "
    echo "                         see CMakePresets.json for the full list."
    echo "  --target <name>        Specifies the target to build (case-sensitive), defaults to \"all\","
    echo "                         can also be \"install\" or a specific project target."
    echo "  --rebuild              If this switch is supplied, the --clean-first parameter will be used to"
  printf "                         run a clean re-build.\n\n"
  printf "  --tests                If this switch is supplied tests will be run.\n\n"

  exit 1
}

preset_name="release-linux"
config_name=""
config_param=""
clean_param=""
target="all"
rebuild=false
run_tests=false

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
    --config)
      config_name=$2
      shift
      ;;
    --rebuild)
      clean_param="--clean-first"
      ;;
    --tests)
      run_tests=true
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

if ($config_name)
   $config_param="--config=$config_name"
fi
echo "Building preset $preset_name $config_name using repo dir $repo_dir..."

cmake --build --preset=$preset_name $config_param --target=$target $clean_param


if $run_tests; then
   config_param="--build-config $config_name"
   ctest --preset $preset_name --output-on-failure --output-junit "$preset_name.test_results.xml"
fi
cd "$saved_location"
