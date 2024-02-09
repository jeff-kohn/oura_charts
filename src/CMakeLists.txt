
###############################################################################
# Target oura_rest
###############################################################################

set(THIS_TARGET "oura_rest")

add_library(${THIS_TARGET} STATIC
	"../include/oura_charts/RestAuth.h"
	"../include/oura_charts/UserProfile.h"
	"../include/oura_charts/detail/rest_constants.h"
	"bind_mappings.cpp"
 "RestAuth.cpp")

set_target_properties( ${THIS_TARGET} 
	PROPERTIES
		OUTPUT_NAME ${THIS_TARGET}
		COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMPILE_PDB_NAME ${THIS_TARGET}
)

target_include_directories( ${THIS_TARGET}
	PUBLIC 
		${fmt_INCLUDE_DIRS}
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE:
		${Boost_INCLUDE_DIRS}
)

target_compile_features(${THIS_TARGET} PUBLIC ${PROJECT_CXX_STANDARD})
target_compile_options(${THIS_TARGET} PRIVATE ${PROJECT_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${PROJECT_WARNING_FLAGS})
target_compile_definitions(${THIS_TARGET} PRIVATE ${PROJECT_COMPILE_DEFINITIONS})
