#include "OuraChartsApp.h"
#include "MainFrame.h"

namespace oura_charts
{

   bool OuraChartsApp::OnInit()
   {
   	auto *main_frame = new MainFrame(nullptr);
   	main_frame->Show(true);
   	SetTopWindow(main_frame);
   	return true;
   }

   wxIMPLEMENT_APP(OuraChartsApp);

}