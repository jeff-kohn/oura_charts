///////////////////////////////////////////////////////////////////////////////
// Code generated by wxUiEditor - see https://github.com/KeyWorksRW/wxUiEditor/
//
// Do not edit any code above the "End of generated code" comment block.
// Any changes before that block will be lost if it is re-generated!
///////////////////////////////////////////////////////////////////////////////

// clang-format off

#pragma once

#include <wx/choice.h>
#include <wx/dialog.h>
#include <wx/gdicmn.h>
#include <wx/listbox.h>

namespace oura_charts
{
    class AddDataFieldBase : public wxDialog
    {
    public:
        AddDataFieldBase() {}
        AddDataFieldBase(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString& title = "Add Data Field",
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE, const wxString &name = wxDialogNameStr)
        {
            Create(parent, id, title, pos, size, style, name);
        }

        bool Create(wxWindow *parent, wxWindowID id = wxID_ANY, const wxString& title = "Add Data Field",
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_DIALOG_STYLE, const wxString &name = wxDialogNameStr);

    protected:

        // Class member variables

        wxChoice* m_data_source_cbo;
        wxListBox* m_field_list;
        wxListBox* m_function_list;
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
