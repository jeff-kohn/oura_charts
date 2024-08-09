
#pragma once

#include "constants.h"

#include <wx/event.h>
#include <wx/docview.h>
#include <wx/sharedptr.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>


#include <memory>




namespace oura_charts
{

   class ChartCanvas;

   class MainFrame : public wxDocParentFrame
   {
   public:
      // If you use default constructor, you must call Create(parent*)
      MainFrame() = default;
      explicit MainFrame(const std::weak_ptr<wxDocManager>& doc_mgr, wxFrame* parent,
                         const wxPoint& pos = wxDefaultPosition,
                         const wxSize& size = wxDefaultSize,
                         long style = wxDEFAULT_FRAME_STYLE,
                         wxWindowID id = wxID_ANY,
                         const wxString& title = constants::APP_NAME)
      {
         Create(doc_mgr, parent);
      }

      // cppcheck-suppress duplInheritedMember
      bool Create(const std::weak_ptr<wxDocManager>&doc_mgr, wxFrame* parent,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxDEFAULT_FRAME_STYLE,
                  wxWindowID id = wxID_ANY,
                  const wxString& title = constants::APP_NAME);


   protected:
      wxBoxSizer* m_sizer_horiz{};
      wxBoxSizer* m_sizer_vert{};
      wxStatusBar* m_statusBar{};
      wxToolBar* m_toolbar{};
      ChartCanvas* m_canvas{};

      void initControls();
      void onMenuFilePreferences(wxCommandEvent& event);
      void onMenuFileQuit(wxCommandEvent& event);
      void onMenuFileTestChart(wxCommandEvent& event);
      void onMenuHelpAbout(wxCommandEvent& event);
      void onMenuHelpAboutWx(wxCommandEvent& event);
   };


}  // namespace oura_charts
