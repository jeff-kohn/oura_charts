///////////////////////////////////////////////////////////////////////////////
// Code generated by wxUiEditor - see https://github.com/KeyWorksRW/wxUiEditor/
//
// Do not edit any code above the "End of generated code" comment block.
// Any changes before that block will be lost if it is re-generated!
///////////////////////////////////////////////////////////////////////////////

// clang-format off

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/menu.h>

#include "MainFrameBase.h"

using namespace oura_charts;

bool MainFrameBase::Create(wxWindow* parent, wxWindowID id, const wxString& title,
    const wxPoint& pos, const wxSize& size, long style, const wxString &name)
{

    if (!wxFrame::Create(parent, id, title, pos, size, style, name))
        return false;

    m_toolbar = CreateToolBar();
    m_toolbar->Realize();

    auto* menubar = new wxMenuBar();

    auto* m_file_menu = new wxMenu();
    auto* menu_item2 = new wxMenuItem(m_file_menu, wxID_PREFERENCES);
    m_file_menu->Append(menu_item2);
    m_file_menu->AppendSeparator();
    auto* menu_item = new wxMenuItem(m_file_menu, wxID_EXIT);
    menu_item->SetBitmap(wxArtProvider::GetBitmapBundle(wxART_QUIT, wxART_MENU));

    m_file_menu->Append(menu_item);
    menubar->Append(m_file_menu, wxGetStockLabel(wxID_FILE));

    auto* m_menu = new wxMenu();
    auto* menu_item3 = new wxMenuItem(m_menu, wxID_ABOUT);
    m_menu->Append(menu_item3);
    menubar->Append(m_menu, "&Help");

    SetMenuBar(menubar);

    m_statusBar = CreateStatusBar();

    Centre(wxBOTH);

    // Event handlers
    Bind(wxEVT_MENU, &MainFrameBase::OnMenuHelpAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrameBase::onMenuFilePreferences, this, wxID_PREFERENCES);

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
