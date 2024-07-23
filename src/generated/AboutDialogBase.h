///////////////////////////////////////////////////////////////////////////////
// Code generated by wxUiEditor - see https://github.com/KeyWorksRW/wxUiEditor/
//
// Do not edit any code above the "End of generated code" comment block.
// Any changes before that block will be lost if it is re-generated!
///////////////////////////////////////////////////////////////////////////////

// clang-format off

#pragma once

#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

namespace oura_charts
{
    class AboutDialogBase : public wxDialog
    {
    public:
        static const int form_id = wxID_ANY;
        static const int form_style = wxDEFAULT_DIALOG_STYLE;
        static const wxPoint form_pos() { return wxDefaultPosition; }
        static const wxSize form_size() { return  wxSize(250, 175); }
        static const wxString form_title() { return wxString::FromUTF8("About Oura Charts"); }

        AboutDialogBase() {}
        AboutDialogBase(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString& title = "About Oura Charts",
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(250, 175),
            long style = wxDEFAULT_DIALOG_STYLE, const wxString &name = wxDialogNameStr)
        {
            Create(parent, id, title, pos, size, style, name);
        }

        bool Create(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString& title = "About Oura Charts",
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(250, 175),
            long style = wxDEFAULT_DIALOG_STYLE, const wxString &name = wxDialogNameStr);

        void setProfileText(const wxString& value) { m_profile_text = value; }

    protected:

        // Validator variables

        wxString m_profile_text;

        // Class member variables

        wxBoxSizer* m_main_sizer;
        wxStaticText* m_lbl_user_profile;
    };
} // namespace oura_charts


// ************* End of generated code ***********
// DO NOT EDIT THIS COMMENT BLOCK!
//
// Code below this comment block will be preserved
// if the code for this class is re-generated.
//
// clang-format on
// ***********************************************
