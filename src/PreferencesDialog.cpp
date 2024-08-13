//---------------------------------------------------------------------------------------------------------------------
// PreferencesDialog.cpp
//
// source file for the PreferencesDialog class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

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
            m_have_saved_token = true;
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


   void PreferencesDialog::onAccessTokenTextChanged(wxCommandEvent&)
   {
      // reset test state, since the PAT value changed.
      m_successful_test = false;
   }


   void PreferencesDialog::onSaveClicked(wxCommandEvent&)
   {
      using namespace constants;
      try
      {
         if (!Validate() or !TransferDataFromWindow())
            return;

         auto secret_store = wxSecretStore::GetDefault();
         if (!secret_store.IsOk())
            throw oura_exception{ MSG_ERROR_NO_PAT };

         if (m_have_saved_token and m_access_token_val.empty())
         {
            // Confirm user really wants to delete saved value.
            if (wxYES != wxMessageBox(MSG_RESET_TOKEN_PROMPT, TITLE_RESET_TOKEN_PROMPT, wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION))
               return;

            // Delete the saved value.
            if (!secret_store.Delete(CONFIG_VALUE_PAT_VAR))
               throw oura_exception{ MSG_ERROR_DELETE_SECRET_FAILED };
         }
         else
         {
            if (secret_store.Save(CONFIG_VALUE_PAT_VAR, wxString{}, wxSecretValue{ m_access_token_val }))
               throw oura_exception{ MSG_ERROR_SAVE_SECRET_FAILED };
         }
         EndDialog(wxID_SAVE);
      }
      catch(std::exception& e)
      {
         auto msg = std::format(FMT_MSG_ERROR_SAVE_SECRET_FAILED, e.what());
         wxLogError(msg.c_str());
      }
   }


   void PreferencesDialog::onSaveUpdateUI(wxUpdateUIEvent& event)
   {
      // We only want to enable Save if:
      //  * User has entered a token value and successfully tested it.
      //  * User has deleted the existing text, to delete an existing saved token.
      //
      // If current text == PASSWORD_DISPLAY, then we have saved token, but the user
      // hasn't changed it so there's no need to enable Save.
      //
      auto text = m_access_token_txt->GetValue();
      event.Enable( (text.empty() and m_have_saved_token) or (m_successful_test && text != constants::PASSWORD_DISPLAY) );
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


}  // namespace oura_charts
