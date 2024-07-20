#include "OuraChartsApp.h"
#include "MainFrame.h"

namespace oura_charts
{
   OuraChartsApp::OuraChartsApp()
   {
      SetUseBestVisual(true);
   }

   bool OuraChartsApp::OnInit()
   {
   	auto *main_frame = new MainFrame(nullptr);
   	main_frame->Show(true);
   	SetTopWindow(main_frame);
   	return true;
   }
}

// this needs to be outside the namespace for Linux but now Windows, go figure
wxIMPLEMENT_APP(oura_charts::OuraChartsApp);