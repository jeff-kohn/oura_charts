#include "OuraChartsApp.h"
#include "ChartDocTemplate.h"
#include "MainFrame.h"

#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/secretstore.h>

#include "oura_charts/detail/utility.h"
#include <chrono>

namespace oura_charts
{
   namespace
   {
      const wxSize g_default_frame_size{ 400, 600 };
   }


   using std::make_unique;

   OuraChartsApp::OuraChartsApp()
   {
      SetUseBestVisual(true);

      // Set up config object to use file even on windows (registry is yuck)
      wxStandardPaths::Get().SetFileLayout(wxStandardPaths::FileLayout::FileLayout_XDG);
      wxConfigBase::DontCreateOnDemand();
      auto cfg = make_unique<wxFileConfig>(constants::APP_NAME_NOSPACE, 
                                           wxEmptyString, 
                                           wxEmptyString,  
                                           wxEmptyString,  
                                           wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_SUBDIR);
      wxConfigBase::Set(cfg.release());
   } // NOLINT(clang-analyzer-cplusplus.NewDeleteLeaks) unfortunately no way around it with wxWindows


   bool OuraChartsApp::OnInit()
   {
      if (!wxApp::OnInit())
         return false;

      ::wxInitAllImageHandlers();

      assert(!m_doc_mgr);
      m_doc_mgr = std::make_shared<wxDocManager>();
      m_doc_mgr->SetMaxDocsOpen(1);

      new ChartDocTemplate(m_doc_mgr.get(), "Oura Charts Document",
                           "*.occhart", wxEmptyString, "occhart",
                           "OC Chart Doc", "OC Chart View");

      auto main_frame = std::make_unique<MainFrame>(m_doc_mgr,
                                                    nullptr,
                                                    wxDefaultPosition,
                                                    g_default_frame_size);

      main_frame->Center();
      main_frame->Show();
      SetTopWindow(main_frame.release());

      return true;
   } 


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
      auto secret_store = wxSecretStore::GetDefault();
      wxString user{}, pat{};
      wxSecretValue token{};
      if (secret_store.Load(constants::CONFIG_VALUE_PAT_VAR, user, token))
      {
         pat = token.GetAsString();
      }
      else{
         // try to get it from environment variable.
         pat = detail::getEnvironmentVariable(constants::CONFIG_VALUE_PAT_VAR);
      }

      if (pat.empty())
         return unexpected{ oura_exception{ ErrorCategory::REST, constants::FMT_MSG_ERROR_NO_PAT, constants::CONFIG_VALUE_PAT_VAR } };

      return TokenAuth{ pat.ToStdString() };
   }

}  // namespace oura_charts

// this needs to be outside the namespace for Linux but not Windows, go figure
wxIMPLEMENT_APP(oura_charts::OuraChartsApp);
