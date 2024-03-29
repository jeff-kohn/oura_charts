# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)

project ("oura_charts"
   VERSION 1.0
   DESCRIPTION "Charting Library and Rest Client for Oura Ring"
   LANGUAGES CXX
   HOMEPAGE_URL "https://github.com/jeff-kohn/oura-charts"
 )

 if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
   # So we can enable/disable build feature depending whether this is top-level build
   set(TOP_LEVEL_PROJECT ON)
endif()


# So that cmake can find any additional modules we've written (stuff like Find modules, export targets, etc)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/extras/")


# Only do these if this is the main project, and not if it is included through add_subdirectory
if (TOP_LEVEL_PROJECT)
   # Let's ensure -std=c++xx instead of -std=g++xx
   set(CMAKE_CXX_EXTENSIONS OFF)

   # Let's nicely support folders in IDEs
   set_property(GLOBAL PROPERTY USE_FOLDERS ON)

   # Different presets may enable/disable code analysis tools
   add_custom_target (code_analysis DEPENDS all)
   include (cppcheck/UseCppCheck)
   include (clang-tidy/UseClangTidy)

   if (BUILD_TESTING)
      enable_testing()
      find_package(Catch2 3 REQUIRED)
   endif()
endif()


if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR) 
   message(FATAL_ERROR "In-source builds are not allowed") 
endif()


# Set the C++ standard. All our targets will use this, so we can easily change it
# if desired in the future.
set(PROJECT_CXX_STANDARD cxx_std_23)

#
# Set up platform-specific target options, by creating the vars that will be used to call target_compile_options/definitions
# for each target. If any of these variables have already been defined by a preset, we will NOT override the preset values.
#
# This allows us to have centralized control over these settings and individual targets can still choose to override them 
# if necessary. And we're following modern best practices of avoiding changing CMAKE_CXX_FLAGS or add_compile_options()
#
# We take care of setting the appropriate flags for debug/release and windows/linux. If you override these settings with a preset 
# you need to do the same. Also make sure your variable values end with a semi-colon so that additional values can be appended 
# if needed.
#
# We only support MSCV on Windows and CLANG on Linux for now. 
#
if (WIN32)
    # By defualt cmake is using linux conventions (eg libname instead of name.lib) so override that for Windows builds.
    set(CMAKE_FIND_LIBRARY_PREFIXES "")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")

    # Set the preprocessor flags.
    if(NOT DEFINED PROJECT_COMPILE_DEFINITIONS)
        set(PROJECT_COMPILE_DEFINITIONS "_WINSOCK_DEPRECATED_NO_WARNINGS;NOMINMAX;_WIN32_WINNT=_WIN32_WINNT_WIN7;WIN32_LEAN_AND_MEAN;")
        string(APPEND PROJECT_COMPILE_DEFINITIONS  "$<$<CONFIG:DEBUG>:_DEBUG;_ALLOW_RTCc_IN_STL>"
                                                   "$<$<CONFIG:RelWithDebInfo>:NDEBUG;>"
                                                   "$<$<CONFIG:Release>:NDEBUG;>")
    endif()

    # Set the warning flags 
    if(NOT DEFINED PROJECT_WARNING_FLAGS)
        set(PROJECT_WARNING_FLAGS "/W4;/w14263;/w14265;/w14311;/w14555;/w14640;/w14928;/wd4100;/WX")
    endif()

    # Set other compiler flags
    if(NOT DEFINED PROJECT_COMPILE_OPTIONS)
        set(PROJECT_COMPILE_OPTIONS "/MP;/cgthreads8;/sdl;/GS;/Zc:wchar_t;/Zc:inline;/permissive-;/utf-8;/EHsc;/Zf;/Zi;/Zc:__cplusplus;/Zc:strictStrings;/Zf;/Zc:templateScope;/Zc:throwingNew;")
        string(APPEND PROJECT_COMPILE_OPTIONS  "$<$<CONFIG:Debug>:/Od;/EHr;/RTC1>"
                                               "$<$<CONFIG:Release>:/O2;/GL;/Gw;/Gy;>"
                                               "$<$<CONFIG:RelWithDebInfo>:/O2;/GL;/Gw;/Gy;>")
    endif()

    # Set linker flags
    if(NOT DEFINED PROJECT_LINK_OPTIONS)
        set(PROJECT_LINK_OPTIONS "/DYNAMICBASE;/NXCOMPAT; /CGTHREADS:8")
        string(APPEND PROJECT_LINK_OPTIONS  "$<$<CONFIG:DEBUG>:/INCREMENTAL;/OPT:NOICF;>"
                                            "$<$<CONFIG:Release>:/Gw;/Gy;/OPT:ICF;/OPT:REF;/LTCG:incremental;>"
                                            "$<$<CONFIG:RelWithDebInfo>:/Gw;/Gy;/OPT:ICF;/OPT:REF;/LTCG:incremental;>")
    endif()

elseif (LINUX)

endif()

include(CMakePrintHelpers)

find_package(Boost REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(cxxopts CONFIG REQUIRED)
find_package(restc-cpp CONFIG REQUIRED)

cmake_print_variables(PROJECT_CXX_STANDARD)
cmake_print_variables(PROJECT_COMPILE_DEFINITIONS)
cmake_print_variables(PROJECT_WARNING_FLAGS)
cmake_print_variables(PROJECT_COMPILE_OPTIONS)
cmake_print_variables(PROJECT_LINK_OPTIONS)

# Include sub-projects.
//add_subdirectory("src")


