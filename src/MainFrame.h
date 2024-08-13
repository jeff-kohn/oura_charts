//---------------------------------------------------------------------------------------------------------------------
// MainFrame.h
//
// header file for the MainFrame window class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "constants.h"
#include "forward_declarations.h"

#include <wx/event.h>
#include <wx/docview.h>
#include <wx/sharedptr.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>




namespace oura_charts
{

   /// <summary>
   ///   class MainFrame is the main window for the application. It owns the menu, status bar,
   ///   and the window that is used for displaying the view for our document class.
   /// </summary>
   class MainFrame final : public wxDocParentFrame 
   {
   public:
      // If you use default constructor, you must call Create(parent*)
      MainFrame(const DocManagerPtrWk& doc_mgr, wxFrame* parent,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                wxWindowID id = wxID_ANY,
                const wxString& title = constants::APP_NAME)
      {
         Create(doc_mgr, parent, pos, size, style, id, title);
      }


      ChartOptionsCanvas* getCanvas()
      {
         return m_canvas;
      }

   private:
      ChartOptionsCanvas* m_canvas{};
      wxStatusBar*        m_statusBar{};
      wxMenu*             m_edit_menu{};

      void initControls();
      void onMenuFilePreferences(wxCommandEvent& event);
      void onMenuFileQuit(wxCommandEvent& event);
      void onMenuFileTestChart(wxCommandEvent& event);
      void onMenuHelpAbout(wxCommandEvent& event);
      void onMenuHelpAboutWx(wxCommandEvent& event);

      // cppcheck-suppress duplInheritedMember
      bool Create(const DocManagerPtrWk& doc_mgr,
                  wxFrame* parent,
                  const wxPoint& pos,
                  const wxSize& size,
                  long style,
                  wxWindowID id,
                  const wxString& title);

   };


}  // namespace oura_charts
