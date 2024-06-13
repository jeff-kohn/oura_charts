#!/bin/bash

show_help() {
  printf "\nUsage: %s [options]\n" "$0"
    echo "Trim fully qualified paths in a file to be relative to the specifyed repo folder (or current path)"
    echo "by removing all occurrences of the specified string (eg repo path). Can be used for removing any substring"
    echo "from a text file, it doesn't actually have to be a path"
    printf "\n\nOptions:\n"
    echo "  --input-file <path>  Specifies the file to process"
    echo "  --repo-path <path>   Path/substring that should be removed from the file. Defaults to current path"
    echo "                       if not specified."
    echo "  --output-file <path> Specifies the target file to write result to. If not specified, file will be"
    echo "                       edited 'in-place'"
    echo "  --backup             Save original file contents to a backup. Only applies when "
    echo "                       editing in-place (no --output-file specified)"
  exit 1
}

main ()
{
   input_file=""
   output_file=""
   backup=false
   repo_path="$(pwd)"
   in_place_edit=true

   while [ $# -gt 0 ]; do
      case "$1" in
         --input-file)
            input_file=$2
            shift
            ;;
         --repo-path)
            repo_path=$2
            shift
            ;;
         --output-file)
            output_file=$2
            in_place_edit=false
            shift
            ;;
         --backup)
            backup=true
            shift
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

   if [[ "${repo_path: -1}" != "/" ]]; then
      repo_path="$repo_path/"
   fi

   echo "Removing all occurrences of $repo_path from $input_file..."

   # Figure out whether we should do in-place edit or save results to a new file.
   # Backup file option only supported for in-place edit, and only if backup
   # filename was specified.
   if $in_place_edit; then
      if $backup; then
         sed --expression="s|file://$repo_path||g; s|$repo_path||g" -i.backup "$input_file"
      else
         sed --expression="s|file://$repo_path||g; s|$repo_path||g" -i "$input_file"
      fi
   else
       sed --expression="s|file://$repo_path||g; s|$repo_path||g" "$input_file" > "$output_file"
   fi
}

main "$@"
