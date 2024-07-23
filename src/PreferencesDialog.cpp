
#include "PreferencesDialog.h"
#include "OuraChartsApp.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/UserProfile.h"
#include <wx/msgdlg.h>

namespace oura_charts
{
   PreferencesDialog::PreferencesDialog(wxWindow* parent)
   {
      try
      {
         auto secret_store = wxSecretStore::GetDefault();
         wxString unused{};
         wxSecretValue token{};
         if (secret_store.Load(constants::APP_NAME_NOSPACE, unused, token))
         {
            m_has_saved_token = true;
            m_access_token_val = constants::EMPTY_PASSWORD_DISPLAY;
         }
      }
      catch (std::exception& e)
      {
         auto msg = std::format("Unable to authenticate with the supplied token. {}.", e.what());
         wxLogError(msg.c_str());
      }

      Create(parent);

      // set initial focus to text box, not hyperlink label.
      m_access_token_txt->SetFocus();
      CallAfter([this] { m_access_token_txt->SelectAll(); });
   }


   void PreferencesDialog::onSaveClicked(wxCommandEvent& event)
   {
      try
      {
         // save PAT to secret store and dismiss dialog.
         auto secret_store = wxSecretStore::GetDefault();
         secret_store.Save(constants::APP_NAME_NOSPACE, wxEmptyString, wxSecretValue{ m_access_token_val });
         EndDialog(wxID_SAVE);
      }
      catch(std::exception& e)
      {
         auto msg = std::format("Unable to authenticate with the supplied token. {}.", e.what());
         wxLogError(msg.c_str());
      }
   }


   void PreferencesDialog::OnSaveUpdateUI(wxUpdateUIEvent& event)
   {
      // Make sure user has successfully tested a valid PAT
      event.Enable(m_successful_test && event.GetText() != constants::EMPTY_PASSWORD_DISPLAY);
   }


   void PreferencesDialog::onTestClicked(wxCommandEvent& event)
   {
      if (!Validate() or !TransferDataFromWindow())
         return;

      try
      {
         // try to use the supplied token value to authenticate with Oura
         TokenAuth auth{ m_access_token_val.ToStdString() };
         RestDataProvider<TokenAuth> prov{ auth, constants::REST_DEFAULT_BASE_URL };
         auto profile = getUserProfile(prov);

         m_successful_test = true;
         wxMessageDialog dlg(this, std::format("Profile for {} successfully retrieved from Oura REST server", profile.email()).c_str());
         dlg.ShowModal();
      }
      catch (std::exception& e)
      {
         auto msg = std::format("Unable to authenticate with the supplied token. {}.", e.what());
         wxLogError(msg.c_str());
      }
   }


   void PreferencesDialog::OnTestUpdateUI(wxUpdateUIEvent& event)
   {
      // Don't enable test button unless user has entered a value.
      auto str = m_access_token_txt->GetLineText(0);
      event.Enable(!str.empty() && str != constants::EMPTY_PASSWORD_DISPLAY);
   }


   void PreferencesDialog::OnAccessTokenTextChanged(wxCommandEvent& event)
   {
      // reset test state, since the PAT value changed.
      m_successful_test = false;
   }

}  // namespace oura_charts
