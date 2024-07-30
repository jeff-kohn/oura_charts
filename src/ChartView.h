#pragma once

#include <wx/docview.h>

namespace oura_charts
{

   class ChartView : public wxView
   {
   public:

   protected:
      void OnDraw(wxDC* dc) override;
      void OnUpdate(wxView* sender, wxObject* hint = nullptr) override;
      bool OnClose(bool deleteWindow = true) override;
   };

} // namespace oura_charts
