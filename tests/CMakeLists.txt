
###############################################################################
# Target tests
###############################################################################
set(THIS_TARGET "tests")
add_executable(${THIS_TARGET}
   "TestDataProvider.h"
   "TestDataProvider.cpp"
   "test_chrono_helpers.cpp"
   "test_DailySleepScore.cpp"
   "test_functors.cpp"
   "test_HeartRate.cpp"
   "test_oura_exception.cpp"
   "test_SleepSession.cpp"
   "test_UserProfile.cpp"
 )

set_target_properties(${THIS_TARGET}
	PROPERTIES
		OUTPUT_NAME ${THIS_TARGET}
		COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMPILE_PDB_NAME ${THIS_TARGET}
      MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:${OURACHARTS_MSVCRT_DEBUG}>$<$<CONFIG:Release,RelWithDebInfo>:${OURACHARTS_MSVCRT_RELEASE}>"
)

target_link_libraries(${THIS_TARGET}
   PUBLIC
      oura_lib
      Catch2::Catch2WithMain
)

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_link_options(${THIS_TARGET} PRIVATE ${OURACHARTS_LINK_OPTIONS})

# This needs to be in CMakeLists of the directory containing the test target, not project root.
set(CMAKE_CATCH_DISCOVER_TESTS_DISCOVERY_MODE "PRE_TEST")
catch_discover_tests(${THIS_TARGET})

# copy test data for unit tests to outut dir.
add_custom_command(
   TARGET ${THIS_TARGET} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/test_data/
            ${CMAKE_CURRENT_BINARY_DIR}/test_data/
            #$<TARGET_FILE_DIR:${THIS_TARGET}>/test_data/
)

