
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

   class MainFrame : public wxDocParentFrame
   {
   public:
      // If you use default constructor, you must call Create(parent*)
      MainFrame() = default;
      explicit MainFrame(const std::weak_ptr<wxDocManager>& doc_mgr, wxFrame* parent)
      {
         Create(doc_mgr, parent);
      }

      // cppcheck-suppress duplInheritedMember
      bool Create(const std::weak_ptr<wxDocManager>&, wxFrame* parent,
                  wxWindowID id = wxID_ANY, const wxString& title = constants::APP_NAME);

   protected:
      wxBoxSizer* m_chart_sizer{};
      wxStatusBar* m_statusBar{};
      wxToolBar* m_toolbar{};

      void initControls();
      void onMenuFilePreferences(wxCommandEvent& event);
      void onMenuFileQuit(wxCommandEvent& event);
      void onMenuHelpAbout(wxCommandEvent& event);
      void onMenuHelpAboutWx(wxCommandEvent& event);
   };


}  // namespace oura_charts
