
#include "MainFrame.h"
#include "AboutDialog.h"
#include "OuraChartsApp.h"
#include "PreferencesDialog.h"

#include "oura_charts/RestDataProvider.h"
#include "oura_charts/UserProfile.h"


namespace oura_charts
{


   void MainFrame::onMenuFilePreferences(wxCommandEvent&)
   {
      try
      {
         PreferencesDialog dlg(this);
         dlg.ShowModal();
      }
      catch (oura_exception& e)
      {
         wxLogError(e.message.c_str());
      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }

   void MainFrame::OnMenuFileQuit(wxCommandEvent& event)
   {
      Close(true);
   }

   void MainFrame::OnMenuHelpAboutWx(wxCommandEvent& event)
   {
      wxInfoMessageBox(this);
   }


   void MainFrame::onMenuHelpAbout(wxCommandEvent&)
   {
      try
      {
         auto auth_res = wxGetApp().getRestToken();
         if (auth_res)
         {
            RestDataProvider provider{ *auth_res, constants::REST_DEFAULT_BASE_URL };
            auto profile = getUserProfile(provider);
            AboutDialog dlg(profile, this);
            dlg.ShowModal();
         }
         else
         {
            AboutDialog dlg(auth_res.error().message.c_str(), this);
            dlg.ShowModal();
         }
      }
      catch (oura_exception& e)
      {
         wxLogError(e.message.c_str());
      }
      catch (std::exception& e)
      {
         wxLogError(e.what());
      }
   }
}  // namespace oura_charts
