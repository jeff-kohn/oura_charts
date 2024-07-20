
#include "MainFrame.h"
#include "AboutDialog.h"

namespace oura_charts
{

   void MainFrame::onMenuHelpAbout(wxCommandEvent& event) 
   {
      AboutDialog dlg(this);
      dlg.ShowModal();
   }

   void MainFrame::onMenuFilePreferences(wxCommandEvent& event)
   {

   }

}  // namespace oura_charts