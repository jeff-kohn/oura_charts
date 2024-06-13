#!/usr/bin/env bash

show_help() {
  printf "\nUsage: %s [options]\n" "$0"
   printf "\nOptions:\n"
    echo "  --source-dir <path>   Specifies the build directory (aka CMAKE_BINARY_DIR)"
    echo "  --build-dir <path>    Specifies the source directory (aka CMAKE_SOURCE_DIR)"
    echo "  --command-path <path> The path to run-clang-tidy. Defaults to 'run-clang-tidy'"
    echo "  --results-file <name> Name of file to save scan results to in the build directory"
    echo "                        Defaults 'clang-tidy-results.sarif'"
    echo "  --trim-paths          If specified, absolute paths in the scan results will be trimmed"
    echo "                        to be relative to the root folder of the repo."
    echo "  --fix                 automatically apply fixes if set (fixes disabled otherwise)"

   printf "\n\n"
  exit 1
}

main ()
{
   source_dir=""
   build_dir=""
   command_path="run-clang-tidy"
   results_file="clang_tidy_results.sarif"
   trim_paths=false
   fix=false

   while [ $# -gt 0 ]; do
      case "$1" in
         --source-dir)
            source_dir=$2
            shift
            ;;
         --build-dir)
            build_dir=$2
            shift
            ;;
         --command-path)
            command_path=$2
            shift
            ;;
         --results-file)
            results_file=$2
            shift
            ;;
         --trim-paths)
            trim_paths=true
            ;;
         --fix)
            fix=true
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
   fq_results_file="$build_dir/$results_file"
   # repo_dir=$(dirname "$script_folder")

   saved_location="$(pwd)"
   cd "$source_dir" || exit

   printf "\nRunning clang-tidy for entire project...\n"
   if $fix; then
      $command_path -header-filter=".*/oura_charts/**" -quiet -p "$build_dir" -fix -style file | clang-tidy-sarif | tee "$fq_results_file" | sarif-fmt
   else
      $command_path -header-filter=".*/oura_charts/**" -quiet -p "$build_dir" | clang-tidy-sarif | tee "$fq_results_file" | sarif-fmt
   fi
   if $trim_paths; then
      # clang-tidy output has fully qualified file paths, we want them to be relative to repo root so that GHAS understands them.
      "$script_folder/trim_repo_paths.sh" --input-file "$fq_results_file" --repo-path "$source_dir" --backup
   fi

   cd $saved_location
}

main "$@"
