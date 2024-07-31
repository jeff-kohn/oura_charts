#pragma once

#include "generated/PreferencesDialogBase.h"
#include <wx/secretstore.h>

namespace oura_charts
{

   class PreferencesDialog : public PreferencesDlgBase
   {
   public:
      // construct dialog with parent and create. we don't need no stinkin' two-phase initialization.
      explicit PreferencesDialog(wxWindow* parent);

      void onSaveClicked(wxCommandEvent& event) override;
      void onSaveUpdateUI(wxUpdateUIEvent& event) override;

      void onTestClicked(wxCommandEvent& event) override;
      void onTestUpdateUI(wxUpdateUIEvent& event) override;

      void onAccessTokenTextChanged(wxCommandEvent& event) override;

   private:
      bool m_has_saved_token{ false };
      bool m_successful_test{ false };
   };                                                  


}  // namespace oura_charts
