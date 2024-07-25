#include "OuraChartsApp.h"
#include "MainFrame.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>

#include "oura_charts/detail/utility.h"
#include <chrono>

namespace oura_charts
{
   OuraChartsApp::OuraChartsApp()
   {
      SetUseBestVisual(true);

      // Set up config object to use file even on windows (registry is yuck)
      wxStandardPaths::Get().SetFileLayout(wxStandardPaths::FileLayout::FileLayout_XDG);
      wxConfigBase::DontCreateOnDemand();
      wxConfigBase::Set(new wxFileConfig{ constants::APP_NAME_NOSPACE, // NOLINT(cppcoreguidelines-owning-memory)
                                          wxEmptyString, 
                                          wxEmptyString,  
                                          wxEmptyString,  
                                          wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR }); 
   } // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)

   bool OuraChartsApp::OnInit()
   {
   	auto *main_frame = new MainFrame(nullptr); // NOLINT(cppcoreguidelines-owning-memory)
   	main_frame->Show(true);
   	SetTopWindow(main_frame);
   	return true;
   } // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks)

   int OuraChartsApp::OnExit()
   {

   #ifdef _DEBUG
      // to prevent the tzdb allocations from being reported as memory leaks
      std::chrono::get_tzdb_list().~tzdb_list();
   #endif

      return wxApp::OnExit();
   }

   wxConfigBase& OuraChartsApp::getConfig() noexcept(false)
   {
      auto *config = wxConfigBase::Get(false);
      if (nullptr == config)
         throw oura_exception{ "No configuration object available" };

      return *config;
   }

   const wxConfigBase& OuraChartsApp::getConfig() const noexcept(false)
   {
      auto *config = wxConfigBase::Get(false);
      if (nullptr == config)
         throw oura_exception{ "No configuration object available" };

      return *config;
   }


   OuraChartsApp::TokenResult OuraChartsApp::getRestToken() const
   {
      auto& config = getConfig();
      wxConfigPathChanger changer(&config, constants::CONFIG_SECTION_REST);

      wxString pat{};
      if (!config.Read(constants::CONFIG_VALUE_REST_PAT, &pat, wxEmptyString))
      {
         // try to get it from enviroment variable.
         pat = detail::getEnvironmentVariable(constants::CONFIG_VALUE_PAT_VAR);
      }

      if (pat.empty())
         return unexpected{ oura_exception{ ErrorCategory::REST, constants::ERROR_MSG_FMT_NO_PAT, constants::CONFIG_VALUE_PAT_VAR } };

      return TokenAuth{ pat.ToStdString() };
   }

}  // namespace oura_charts

// this needs to be outside the namespace for Linux but not Windows, go figure
wxIMPLEMENT_APP(oura_charts::OuraChartsApp);
