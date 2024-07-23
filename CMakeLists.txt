# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)

project ("oura_charts"
   VERSION 0.1.0
   DESCRIPTION "Charting Library and Rest Client for Oura Ring"
   LANGUAGES CXX
   HOMEPAGE_URL "https://github.com/jeff-kohn/oura-charts"
 )

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
   # So we can enable/disable build feature depending whether this is top-level build
   set(TOP_LEVEL_PROJECT ON)
endif()

# So that cmake can find any additional modules we've written (stuff like Find modules, export targets, etc)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tools/")

include(CMakePrintHelpers)

# Set the C++ standard. All our targets will use this, so we can easily change it
# if desired in the future.
set(OURACHARTS_CXX_STANDARD cxx_std_23)

#export compile_commands.json for tools like clang-tidy and cppcheck
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

# Set up platform-specific target options, by creating the vars that will be used to call target_compile_options/definitions
# for each target. If any of these variables have already been defined by a preset, we will NOT override the preset values.
#
# This allows us to have centralized control over these settings and individual targets can still choose to override them
# if necessary. And we're following modern best practices of avoiding changing CMAKE_CXX_FLAGS or add_compile_options()
#
# We take care of setting the appropriate flags for debug/release/relwithdbginfo and windows/linux. If you override these settings
# with a preset you need to do the same. Also make sure your variable values end with a semi-colon so that additional values can be
# appended if needed.
#
# We only support MSCV on Windows and CLANG on Linux for now. Support for other compilers can be added through presets that define
# the appropriate flags.
if (WIN32)

    # By defaUlt cmake is using linux conventions (eg libname instead of name.lib) so override that for Windows builds.
    set(CMAKE_FIND_LIBRARY_PREFIXES "")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

    set(OURACHARTS_COMPILE_DEFINITIONS "_WINSOCK_DEPRECATED_NO_WARNINGS;NOMINMAX;_WIN32_WINNT=_WIN32_WINNT_WIN10;WIN32_LEAN_AND_MEAN;wxUSE_UNICODE_UTF8=1;wxUSE_UNICODE_WCHAR=0"
                                        "wxUSE_UNICODE_UTF8=1"
                                        "wxUSE_UNICODE_WCHAR=0"
                                        "wxUSE_CONFIG_NATIVE=0"
                                        "wxUSE_STL=1"
                                        "wxUSE_STD_CONTAINERS=1"
                                        "wxUSE_STD_STRING=1"
                                        "wxUSE_STL_BASED_WXSTRING=1"
                                        "wxUSE_UNSAFE_WXSTRING_CONV=1"
                                        "$<$<CONFIG:DEBUG>:_DEBUG;_ALLOW_RTCc_IN_STL>"
                                        "$<$<CONFIG:Release,RelWithDebInfo>:NDEBUG;>"
                                        CACHE STRING "Default global preprocessor definitions for oura_charts project")

    set(OURACHARTS_WARNING_FLAGS "/W4;/w14186;/w14263;/w14265;/w14296;/w14311;/w14545;/w14547;/w14549;/w14555;/w14640;/w14826;/w14905;/w14906;/w14928;/wd4100;/external:anglebrackets;/external:W0;/WX"
                                 CACHE STRING "Default global warning options for oura_charts project")

    set(OURACHARTS_COMPILE_OPTIONS "/MP;/cgthreads8;/bigobj;/sdl;/GS;/Zc:wchar_t;/Zc:inline;/permissive-;/utf-8;/EHsc;/Zf;/Zi;/Zc:__cplusplus;/Zc:strictStrings;/Zf;/Zc:templateScope;/Zc:throwingNew;"
                                   "$<$<CONFIG:Debug>:/Od;/EHr;/RTC1>"
                                   "$<$<CONFIG:Release,RelWithDebInfo>:/O2;/GL;/Gw;/Gy;>"
                                   CACHE STRING "Default global compile options for oura_charts project")

    set(OURACHARTS_LINK_OPTIONS "/DYNAMICBASE;/NXCOMPAT;/CGTHREADS:8;"
                                "$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOICF;>"
                                "$<$<CONFIG:Release,RelWithDebInfo>:/OPT:ICF;/OPT:REF;/LTCG:incremental;>"
                                CACHE STRING "Default global linker options for oura_charts project")

    set(OURACHARTS_MSVCRT_DEBUG "MultiThreadedDebug" CACHE STRING "MSVC runtime library version used for debug builds")
    set(OURACHARTS_MSVCRT_RELEASE "MultiThreaded" CACHE STRING "MSVC runtime library version used for release builds")

elseif (LINUX)

    set(OURACHARTS_COMPILE_DEFINITIONS "_GLIBCXX_ASSERTION;wxUSE_UNICODE_UTF8=1;wxUSE_UNICODE_WCHAR=0"
                                       "$<$<CONFIG:DEBUG>:_DEBUG;>"
                                       "$<$<CONFIG:Release,RelWithDebInfo>:NDEBUG;>"
                                       CACHE STRING "Default global preprocessor definitions for oura_charts project")

    set(OURACHARTS_WARNING_FLAGS "-Wall;-Wextra;-Wpedantic;-Werror;" CACHE STRING "Default global compile options for oura_charts project")

    set(OURACHARTS_COMPILE_OPTIONS "$<$<CONFIG:Debug>:-Og;>"
                                   "$<$<CONFIG:Release,RelWithDebInfo>:-O3>"
                                   "-g;-fstack-clash-protection;-fstack-protector-strong;-fexceptions;-fpie;-grecord-gcc-switches;-fcf-protection;-pipe;-Werror=format-security;-frelaxed-template-template-args;-fno-strict-overflow;-Wstrict-overflow"
                                   CACHE STRING "Default global compile options for oura_charts project")

    set(OURACHARTS_LINK_OPTIONS "-pie;" CACHE STRING "Default global linker options for oura_charts project")

   # these are only needed on linux due to incomplete standard library implementations, and should
   # go away at some point.
   find_package(tl-expected CONFIG REQUIRED)
endif()

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
   message(FATAL_ERROR "In-source builds are not allowed")
endif()


# Only do these if this is the main project, and not if it is included through add_subdirectory
if (TOP_LEVEL_PROJECT)
   # Let's ensure -std=c++xx instead of -std=g++xx
   set(CMAKE_CXX_EXTENSIONS OFF)

   # Let's nicely support folders in IDEs
   set_property(GLOBAL PROPERTY USE_FOLDERS ON)

   # Set up the analysis tools and unit testing.
   add_custom_target (code_analysis DEPENDS all)
   include (cppcheck/UseCppCheck)
   include (clang-tidy/UseClangTidy)

   if (ENABLE_UNIT_TESTS)
      enable_testing()
      find_package(Catch2 3 REQUIRED)
      include(CTest)
      include(Catch)
      add_subdirectory ("tests")
  endif()

endif()

include(FetchContent)

FetchContent_Declare(
  glaze
  GIT_REPOSITORY https://github.com/stephenberry/glaze.git
  GIT_TAG main
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glaze)

find_package(Boost REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

# Include sub-projects.
add_subdirectory("lib")
add_subdirectory("src")
add_subdirectory("examples")

cmake_print_variables(OURACHARTS_CXX_STANDARD)
cmake_print_variables(OURACHARTS_COMPILE_DEFINITIONS)
cmake_print_variables(OURACHARTS_WARNING_FLAGS)
cmake_print_variables(OURACHARTS_COMPILE_OPTIONS)
cmake_print_variables(OURACHARTS_LINK_OPTIONS)



