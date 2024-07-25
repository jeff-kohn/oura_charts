
#pragma once

#include "generated/MainFrameBase.h"
#include "constants.h"

#include <wx/sharedptr.h>

class wxChartsCategoricalData;

namespace oura_charts
{
   class MainFrame : public MainFrameBase
   {
   public:
      // If you use default constructor, you must call Create(parent*)
      MainFrame() = default;
      explicit MainFrame(wxWindow* parent)
      {
         Create(parent);
      }

      bool Create(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = constants::APP_NAME,
                  const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
                  long style = wxDEFAULT_FRAME_STYLE, const wxString& name = wxFrameNameStr);

   private:
      void onMenuFilePreferences(wxCommandEvent& event) override;
      void onMenuFileTestChart(wxCommandEvent& event) override;
      void onMenuFileQuit(wxCommandEvent& event) override;
      void onMenuHelpAbout(wxCommandEvent& event) override;
      void onMenuHelpAboutWx(wxCommandEvent& event) override;

      wxSharedPtr<wxChartsCategoricalData> m_data{};
   };


}  // namespace oura_charts
