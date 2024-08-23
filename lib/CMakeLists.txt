
###############################################################################
# Target oura_lib
###############################################################################

set(THIS_TARGET "oura_lib")

add_library(${THIS_TARGET} STATIC
	"../include/oura_charts/detail/utility.h"
	"../include/oura_charts/detail/json_structs.h"
	"../include/oura_charts/detail/logging.h"
	"../include/oura_charts/constants.h"
	"../include/oura_charts/concepts.h"
   "../include/oura_charts/chrono_helpers.h"
   "../include/oura_charts/DataSeries.h"
   "../include/oura_charts/DailySleepScore.h"
   "../include/oura_charts/DailySleepScoreQuery.h"
   "../include/oura_charts/functors.h"
   "../include/oura_charts/HeartRate.h"
   "../include/oura_charts/oura_charts.h"
	"../include/oura_charts/oura_exception.h"
   "../include/oura_charts/RestDataProvider.h"
   "../include/oura_charts/SleepSession.h"
	"../include/oura_charts/TokenAuth.h"
	"../include/oura_charts/UserProfile.h"
   "../include/oura_charts/SchemaManager.h"
   "../include/oura_charts/schema/heart_rate_schema.json"
   "../include/oura_charts/schema/daily_sleep_score_schema.json"
   "DailySleepScoreQuery.cpp"
   "logging.cpp"
   "SchemaManager.cpp"
   "utility.cpp"
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
      ${CPR_INCLUDE_DIRS}
      ${fmt_INCLUDE_DIRS}
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		${Boost_INCLUDE_DIRS}
)

# Since this is a static lib, we make all our dependencies public because
# users of lib will need to link against them 
target_link_libraries(${THIS_TARGET}
   PUBLIC
      cpr::cpr
      fmt::fmt
      glaze::glaze
)

# Need this for now due to incomplete libstdc++
if (LINUX)
   target_link_libraries(${THIS_TARGET} PRIVATE tl::expected)
endif()

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})

