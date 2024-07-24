###############################################################################
# Target OuraCharts
###############################################################################
set(THIS_TARGET "OuraCharts")

include(wx_generated_source.cmake)

add_executable(${THIS_TARGET} WIN32
	AboutDialog.cpp
	AboutDialog.h
	MainFrame.cpp
	MainFrame.h
	OuraChartsApp.h
	OuraChartsApp.cpp
	PreferencesDialog.cpp
	PreferencesDialog.h
   utf8.manifest
	${wx_generated_files}
)

set_target_properties(${THIS_TARGET}
	PROPERTIES
		OUTPUT_NAME ${THIS_TARGET}
		COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMPILE_PDB_NAME ${THIS_TARGET}
      VS_DPI_AWARE "PerMonitor"
      MSVC_RUNTIME_LIBRARY "$<$<CONFIG:Debug>:${OURACHARTS_MSVCRT_DEBUG}>$<$<CONFIG:Release,RelWithDebInfo>:${OURACHARTS_MSVCRT_RELEASE}>"
)

find_package(wxWidgets CONFIG REQUIRED)
find_package(wxcharts CONFIG REQUIRED)

target_link_libraries(${THIS_TARGET}
   PRIVATE
      oura_lib
		wx::core 
		wx::base
      wxcharts::wxcharts
)

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_link_options(${THIS_TARGET} PRIVATE ${OURACHARTS_LINK_OPTIONS})
