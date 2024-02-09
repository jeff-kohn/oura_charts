
# Note we use find_program twice: once for the custom target, which is always enabled if CppCheck is found and performs a full project scan
# saving the results to an XML file; and once for the integrated build check, where CMake calls CppCheck for each file when it's compiled. 
# We only want to enable the in-build scanning for presets which ask for it.

message (CHECK_START "Looking for CppCheck")

# only do the in-build scanning for the presets that request it.
if (TOTP_ENABLE_CPP_CHECK)
   find_program(CMAKE_CXX_CPPCHECK NAMES cppcheck HINTS $ENV{PROGRAMFILES}/cppcheck)

   if (CMAKE_CXX_CPPCHECK)
      message (CHECK_PASS "CppCheck found at '${CPPCHECK_PATH}'")
      list(
         APPEND CMAKE_CXX_CPPCHECK 
               "--enable=all"
               "--inconclusive"
               "--inline-suppr"
               "--library=${CMAKE_SOURCE_DIR}/extras/cppcheck/googletest.cfg"    # This defines google test macros so that CppCheck doesn't choke on our unit tests.
               "--suppressions-list=${CMAKE_SOURCE_DIR}/extras/cppcheck/cppcheck_suppressions.txt"  # This file is used for surpressing specific checks in specific source files. 
               "--suppress=missingIncludeSystem"
               "--suppress=missingInclude"
               "--suppress=unusedFunction"
               "--suppress=unmatchedSuppression"
               "-I${CMAKE_SOURCE_DIR}/include"
               "--template={file}({line}): warning: ({severity}): {message} [{id}]" 
               "--quiet"
      )
      message(STATUS "CppCheck build integration is enabled for this preset.")
   endif()
else()
   message(STATUS "CppCheck build integration is disabled for this build preset")
endif()

find_program(CPPCHECK_PATH NAMES cppcheck HINTS $ENV{PROGRAMFILES}/cppcheck)
if (CPPCHECK_PATH)
   # Need to create the directory for the cppcheck project database, because cppcheck won't create it automatically if it doesn't exist.
   file ( MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/cppcheck )

   # This custom target is used to run CppCheck on the entire project using compile_commands.json. This is used during CI builds to generate 
   # alerts for GitHub Advanced Security, but can also be run locally. Results are output to an XML file, which in the case of CI builds gets 
   # converted to SARIF format and uploaded to Github. 
   add_custom_command ( TARGET code_analysis 
      COMMAND "${CPPCHECK_PATH}" 
         "--enable=all" 
         "--inconclusive" 
         "--inline-suppr" 
         "--suppress=missingIncludeSystem" 
         "--suppress=unmatchedSuppression" 
         "--suppress=missingInclude" 
         "-I\"${CMAKE_SOURCE_DIR}/include\"" 
         "--template=vs"
         "--cppcheck-build-dir=\"${CMAKE_BINARY_DIR}/cppcheck\"" 
         "--library=\"${CMAKE_SOURCE_DIR}/extras/cppcheck/googletest.cfg\""
         "--xml" 
         "--output-file=${CMAKE_BINARY_DIR}/cppcheck_results.xml" 
         "--project=${CMAKE_BINARY_DIR}/compile_commands.json" 
         "--suppressions-list=${CMAKE_SOURCE_DIR}/extras/cppcheck/cppcheck_suppressions.txt"
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      
   )
   message(STATUS "CppCheck project scanning enabled for target 'code_analysis'")
else()
   message(CHECK_FAIL "CppCheck was not found, 'code_analysis' target will not use CppCheck")
endif()

