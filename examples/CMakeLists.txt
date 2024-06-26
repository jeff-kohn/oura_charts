###############################################################################
# Target get_profile
###############################################################################
set(THIS_TARGET "get_profile")
add_executable(${THIS_TARGET}
   "get_profile.cpp"
 "helpers.h")

set_target_properties(${THIS_TARGET}
	PROPERTIES
		OUTPUT_NAME ${THIS_TARGET}
		COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMPILE_PDB_NAME ${THIS_TARGET}
      MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:${OURACHARTS_MSVCRT_DEBUG}>$<$<CONFIG:Release,RelWithDebInfo>:${OURACHARTS_MSVCRT_RELEASE}>"
)

target_include_directories( ${THIS_TARGET}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(${THIS_TARGET}
   PUBLIC
      oura_rest
)

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_link_options(${THIS_TARGET} PRIVATE ${OURACHARTS_LINK_OPTIONS})


###############################################################################
# Target get_hr_data
###############################################################################
set(THIS_TARGET "get_hr_data")
add_executable(${THIS_TARGET}
   "get_hr_data.cpp"
   "helpers.h"
)

set_target_properties(${THIS_TARGET}
	PROPERTIES
		OUTPUT_NAME ${THIS_TARGET}
		COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMPILE_PDB_NAME ${THIS_TARGET}
      MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:${OURACHARTS_MSVCRT_DEBUG}>$<$<CONFIG:Release,RelWithDebInfo>:${OURACHARTS_MSVCRT_RELEASE}>"
)

target_include_directories( ${THIS_TARGET}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(${THIS_TARGET}
   PUBLIC
      oura_rest
)

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_link_options(${THIS_TARGET} PRIVATE ${OURACHARTS_LINK_OPTIONS})


###############################################################################
# Target get_sleep_data
###############################################################################
set(THIS_TARGET "get_sleep_data")
add_executable(${THIS_TARGET}
   "get_sleep_data.cpp"
   "helpers.h"
)

set_target_properties(${THIS_TARGET}
	PROPERTIES
		OUTPUT_NAME ${THIS_TARGET}
		COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMPILE_PDB_NAME ${THIS_TARGET}
      MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:${OURACHARTS_MSVCRT_DEBUG}>$<$<CONFIG:Release,RelWithDebInfo>:${OURACHARTS_MSVCRT_RELEASE}>"
)

target_include_directories( ${THIS_TARGET}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(${THIS_TARGET}
   PUBLIC
      oura_rest
)

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_link_options(${THIS_TARGET} PRIVATE ${OURACHARTS_LINK_OPTIONS})


