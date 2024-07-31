
#include "PreferencesDialog.h"
#include "OuraChartsApp.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/UserProfile.h"
#include <wx/msgdlg.h>
#include <wx/secretstore.h>

namespace oura_charts
{
   PreferencesDialog::PreferencesDialog(wxWindow* parent)
   {
      try
      {
         auto secret_store = wxSecretStore::GetDefault();
         wxString user{};
         wxSecretValue token{};
         if (secret_store.Load(constants::CONFIG_VALUE_PAT_VAR, user, token))
         {
            m_has_saved_token = true;
            m_access_token_val = constants::PASSWORD_DISPLAY;
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


   void PreferencesDialog::onSaveClicked(wxCommandEvent&)
   {
      try
      {
         auto secret_store = wxSecretStore::GetDefault();

         if (m_has_saved_token and m_access_token_val.empty())
         {
            // Confirm user really wants to delete saved value.
            if (wxID_YES != wxMessageBox(constants::MSG_RESET_TOKEN_PROMPT, constants::TITLE_RESET_TOKEN_PROMPT, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION))
               return;

            // Delete the saved value.
            secret_store.Delete(constants::CONFIG_VALUE_PAT_VAR);
         }
         else
         {
            secret_store.Save(constants::CONFIG_VALUE_PAT_VAR, wxString{}, wxSecretValue{ m_access_token_val });
         }
         EndDialog(wxID_SAVE);
      }
      catch(std::exception& e)
      {
         auto msg = std::format("Unable to authenticate with the supplied token. {}.", e.what());
         wxLogError(msg.c_str());
      }
   }


   void PreferencesDialog::onSaveUpdateUI(wxUpdateUIEvent& event)
   {
      // Make sure user has successfully tested a valid PAT
      std::string text = event.GetText().ToStdString();
      event.Enable( (text.empty() and m_has_saved_token) or (m_successful_test && text != constants::PASSWORD_DISPLAY) );
   }


   void PreferencesDialog::onTestClicked(wxCommandEvent&)
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


   void PreferencesDialog::onTestUpdateUI(wxUpdateUIEvent& event)
   {
      // Don't enable test button unless user has entered a value.
      auto str = m_access_token_txt->GetLineText(0);
      event.Enable(!str.empty() and str != constants::PASSWORD_DISPLAY);
   }


   void PreferencesDialog::onAccessTokenTextChanged(wxCommandEvent&)
   {
      // reset test state, since the PAT value changed.
      m_successful_test = false;
   }

}  // namespace oura_charts
