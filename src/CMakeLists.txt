###############################################################################
# Target OuraCharts
###############################################################################
set(THIS_TARGET "OuraCharts")

include(wx_generated_source.cmake)

add_executable(${THIS_TARGET} WIN32
	"AboutDialog.cpp"
	"AboutDialog.h"
   "AddDataFieldDialog.h"
   "AddDataFieldDialog.cpp"
   "ChartDocTemplate.h"
   "ChartDocTemplate.cpp"
   "ChartDocument.h"
   "ChartDocument.cpp"
   "ChartOptionsCanvas.cpp"
   "ChartOptionsCanvas.h"
   "ChartOptionsView.h"
   "ChartOptionsView.cpp"
   "constants.h"
   "doc_view_fwd.h"
   "MainFrame.cpp"
	"MainFrame.h"
	"OuraChartsApp.cpp"
	"OuraChartsApp.h"
	"PreferencesDialog.cpp"
	"PreferencesDialog.h"
   "utf8.manifest"
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
find_package(Matplot++ CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(${THIS_TARGET}
   PRIVATE
      oura_lib
      Matplot++::matplot
		wx::core 
		wx::base
      # wx::gl
      #${OPENGL_LIBRARY}
)

target_compile_features(${THIS_TARGET} PUBLIC ${OURACHARTS_CXX_STANDARD})
target_compile_definitions(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_DEFINITIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_COMPILE_OPTIONS})
target_compile_options(${THIS_TARGET} PRIVATE ${OURACHARTS_WARNING_FLAGS})
target_link_options(${THIS_TARGET} PRIVATE ${OURACHARTS_LINK_OPTIONS})


# copy schema files to build output tree. We'll copy to both Src/ and Tests/, so that
# they're available when running/debugging either the app or unit tests.
add_custom_command(
   TARGET ${THIS_TARGET} POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/../include/oura_charts/schema/
            $<TARGET_FILE_DIR:${THIS_TARGET}>/schema/
)


