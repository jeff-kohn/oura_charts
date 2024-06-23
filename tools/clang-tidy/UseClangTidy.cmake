# Note we use find_program twice: once for the custom target, which is always enabled if clang-tidy is found, and performs a full project scan
# saving the results to a sarif file; and once for the integrated build check, where CMake calls clang-tidy for each file when it's compiled.
# We only want to enable the in-build scanning for presets which ask for it.

# only do the in-built scanning for the presets that request it.
if(ENABLE_CLANG_TIDY)
   message(CHECK_START "Looking for clang-tidy")
   find_program(CMAKE_CXX_CLANG_TIDY NAMES clang-tidy clang-tidy.exe)

   if(CMAKE_CXX_CLANG_TIDY)
      message(CHECK_PASS "clang-tidy found at '${CMAKE_CXX_CLANG_TIDY}'")
      list(
         APPEND CMAKE_CXX_CLANG_TIDY
         "--use-color"
         "--header-filter=.*/oura_charts/**"
         "--quiet"
#         "-p"
#         "${CMAKE_BINARY_DIR}/compile_commands.json"
      )
      message(STATUS "clang-tidy build integration is enabled for this preset.")
   else()
      message(CHECK_FAIL "clang-tidy build integration is enabled, but clang-tidy was not found")
   endif()
else()
   message(STATUS "clang-tidy build integration disabled for this build preset")
endif()

if (UNIX)
   message(CHECK_START "Looking for run-clang-tidy")
   find_program(RUN_CLANG_TIDY NAMES run-clang-tidy run-clang-tidy.py)

   if(RUN_CLANG_TIDY)
      message(CHECK_PASS "run-clang-tidy found at '${RUN_CLANG_TIDY}'")

      # When 'code_analysis' target is built/run, clang-tidy will be run for the entire project using compile_commands.json
      add_custom_command(TARGET code_analysis
         COMMAND "${CMAKE_SOURCE_DIR}/scripts/run_clang_tidy_project_scan.sh"
         "--source-dir"
         "${CMAKE_SOURCE_DIR}"
         "--build-dir"
         "${CMAKE_BINARY_DIR}"
         "--trim-paths"
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      )
      message(STATUS "run-clang-tidy project scanning enabled for target 'code_analysis'")
   else()
      message(CHECK_FAIL "run-clang-tidy was not found, 'code_analysis' target will not use clang-tidy")
   endif()
else()
   message(STATUS "run-clang-tidy project scanning not currently enabled for Windows builds. code_analysis target will only provide project scanning with CppCheck.")
endif()
