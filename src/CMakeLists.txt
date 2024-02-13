
###############################################################################
# Target oura_rest
###############################################################################

set(THIS_TARGET "oura_rest")

add_library(${THIS_TARGET} STATIC
	"../include/oura_charts/RestAuth.h"
	"../include/oura_charts/UserProfile.h"
	"../include/oura_charts/detail/rest_constants.h"
	"../include/oura_charts/helpers.h"
	"bind_mappings.cpp"
	"RestAuth.cpp"
   "UserProfile.cpp"
   "helpers.cpp"
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
	PRIVATE:
		${Boost_INCLUDE_DIRS}
)

target_link_libraries(${THIS_TARGET}
   PUBLIC
      restc-cpp
      fmt::fmt
)

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})
