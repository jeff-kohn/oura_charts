
#include "MainFrame.h"
#include "AboutDialog.h"
#include "OuraChartsApp.h"

#include "oura_charts/RestDataProvider.h"
#include "oura_charts/UserProfile.h"


namespace oura_charts
{

   void MainFrame::onMenuHelpAbout(wxCommandEvent& event) 
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
            AboutDialog dlg(this);
            dlg.ShowModal();
         }
      }
      catch (oura_exception& e)
      {
         wxLogError(e.message.c_str());
      }
   }

   void MainFrame::onMenuFilePreferences(wxCommandEvent& event)
   {

   }

}  // namespace oura_charts
