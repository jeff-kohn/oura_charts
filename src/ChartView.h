#pragma once

#include <wx/docview.h>

namespace oura_charts
{

   class ChartView : public wxView
   {
   public:

   protected:
      virtual void OnDraw(wxDC* dc);
      virtual void OnUpdate(wxView* sender, wxObject* hint = (wxObject*)NULL);
      virtual bool OnClose(bool deleteWindow = true);
   };

} // namespace oura_charts
