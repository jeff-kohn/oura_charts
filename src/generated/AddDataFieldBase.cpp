///////////////////////////////////////////////////////////////////////////////
// Code generated by wxUiEditor - see https://github.com/KeyWorksRW/wxUiEditor/
//
// Do not edit any code above the "End of generated code" comment block.
// Any changes before that block will be lost if it is re-generated!
///////////////////////////////////////////////////////////////////////////////

// clang-format off

#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "AddDataFieldBase.h"

using namespace oura_charts;

bool AddDataFieldBase::Create(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString &name)
{
    if (!wxDialog::Create(parent, id, title, pos, size, style, name))
        return false;

    auto* dlg_sizer = new wxBoxSizer(wxVERTICAL);

    auto* box_sizer = new wxBoxSizer(wxHORIZONTAL);

    auto* box_sizer3 = new wxBoxSizer(wxVERTICAL);

    auto* static_text = new wxStaticText(this, wxID_ANY, "Data Source:");
    box_sizer3->Add(static_text, wxSizerFlags().Border(wxALL));

    box_sizer3->AddSpacer(6);

    auto* static_text2 = new wxStaticText(this, wxID_ANY, "Field:");
    box_sizer3->Add(static_text2, wxSizerFlags().Border(wxALL));

    box_sizer3->AddSpacer(33);

    auto* static_text3 = new wxStaticText(this, wxID_ANY, "Function:");
    box_sizer3->Add(static_text3, wxSizerFlags().Border(wxALL));

    box_sizer->Add(box_sizer3, wxSizerFlags().Border(wxALL));

    auto* box_sizer2 = new wxBoxSizer(wxVERTICAL);

    m_data_source_cbo = new wxChoice(this, wxID_ANY);
    box_sizer2->Add(m_data_source_cbo, wxSizerFlags().Border(wxALL));

    m_field_list = new wxListBox(this, wxID_ANY);
    box_sizer2->Add(m_field_list, wxSizerFlags(1).Expand().Border(wxALL));

    m_function_list = new wxListBox(this, wxID_ANY);
    box_sizer2->Add(m_function_list, wxSizerFlags(1).Expand().Border(wxALL));

    box_sizer->Add(box_sizer2, wxSizerFlags().Border(wxALL));

    dlg_sizer->Add(box_sizer, wxSizerFlags().Border(wxALL));

    auto* stdBtn = CreateStdDialogButtonSizer(wxOK|wxCANCEL);
    dlg_sizer->Add(CreateSeparatedSizer(stdBtn), wxSizerFlags().Expand().Border(wxALL));

    SetSizerAndFit(dlg_sizer);
    Centre(wxBOTH);

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
