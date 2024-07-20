///////////////////////////////////////////////////////////////////////////////
// Code generated by wxUiEditor - see https://github.com/KeyWorksRW/wxUiEditor/
//
// Do not edit any code above the "End of generated code" comment block.
// Any changes before that block will be lost if it is re-generated!
///////////////////////////////////////////////////////////////////////////////

// clang-format off

#include <wx/sizer.h>
#include <wx/valtext.h>

#include "PreferencesDialogBase.h"

using namespace oura_charts;

bool PreferencesDlgBase::Create(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString &name)
{
    if (!wxDialog::Create(parent, id, title, pos, size, style, name))
        return false;

    auto* dlg_sizer = new wxBoxSizer(wxVERTICAL);

    auto* box_sizer = new wxBoxSizer(wxHORIZONTAL);

    auto* box_sizer2 = new wxBoxSizer(wxVERTICAL);

    auto* hyperlink = new wxHyperlinkCtrl(this, wxID_ANY, "Personal Access Token:",
        "https://cloud.ouraring.com/docs/authentication#personal-access-tokens");
    box_sizer2->Add(hyperlink, wxSizerFlags().Center().Border(wxALL));

    m_access_token_txt = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
        ConvertDialogToPixels(wxSize(142, -1)));
    m_access_token_txt->SetHint("Enter the token for your Oura Ring Account");
    m_access_token_txt->SetMaxLength(32);
    m_access_token_txt->SetValidator(wxTextValidator(wxFILTER_ALPHANUMERIC, &m_access_token_val));
    box_sizer2->Add(m_access_token_txt, wxSizerFlags(10).Center().Border(wxALL));

    box_sizer->Add(box_sizer2,
    wxSizerFlags(5).Bottom().Border(wxLEFT|wxRIGHT|wxTOP, wxSizerFlags::GetDefaultBorder()));

    m_btn_test = new wxButton(this, wxID_ANY, "&Test");
    box_sizer->Add(m_btn_test, wxSizerFlags().Bottom().Border(wxALL));

    dlg_sizer->Add(box_sizer, wxSizerFlags().Expand().Border(wxALL));

    auto* stdBtn = new wxStdDialogButtonSizer();
    stdBtn->AddButton(new wxButton(this, wxID_SAVE));
    stdBtn->AddButton(new wxButton(this, wxID_CANCEL));
    stdBtn->Realize();
    dlg_sizer->Add(CreateSeparatedSizer(stdBtn), wxSizerFlags().Expand().Border(wxALL));

    SetSizerAndFit(dlg_sizer);
    Centre(wxBOTH);

    // Event handlers
    Bind(wxEVT_BUTTON, &PreferencesDlgBase::onSaveClicked, this, wxID_SAVE);
    m_btn_test->Bind(wxEVT_BUTTON, &PreferencesDlgBase::onTestClicked, this);

    return true;
}

// ************* End of generated code ***********
// DO NOT EDIT THIS COMMENT BLOCK!
//
// Code below this comment block will be preserved
// if the code for this class is re-generated.
//
// clang-format on
// ***********************************************