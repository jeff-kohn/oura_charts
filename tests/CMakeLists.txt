
###############################################################################
# Target test_UserProfile
###############################################################################
set(THIS_TARGET "tests")
add_executable(${THIS_TARGET}
   test_UserProfile.cpp
)

# we want to statically link everything
set_property(TARGET ${THIS_TARGET} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set_target_properties(${THIS_TARGET}
	PROPERTIES
		OUTPUT_NAME ${THIS_TARGET}
		COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMPILE_PDB_NAME ${THIS_TARGET}
)
target_include_directories(${THIS_TARGET}
	PUBLIC
      oura_rest  
		${fmt_INCLUDE_DIRS}
	PRIVATE:
		${Boost_INCLUDE_DIRS}
)
target_link_libraries(${THIS_TARGET}
   PUBLIC
      oura_rest
      Catch2::Catch2WithMain
)

target_compile_features(${THIS_TARGET} PUBLIC ${PROJECT_CXX_STANDARD})
target_compile_options(${THIS_TARGET} PRIVATE ${PROJECT_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${PROJECT_WARNING_FLAGS})
target_link_options(${THIS_TARGET} PRIVATE ${PROJECT_LINK_OPTIONS})

# This needs to be in CMakeLists of the directory containing the test target, not project root.
catch_discover_tests(tests)

