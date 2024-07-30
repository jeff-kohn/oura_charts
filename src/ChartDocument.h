#pragma once

#include <wx/docview.h>


namespace oura_charts
{

   class ChartDocument : public wxDocument
   {
   public:
      void OnDraw(wxDC* dc);
      void OnUpdate(wxView* sender, wxObject* hint = nullptr);
      bool OnClose(bool deleteWindow = true);
   };

} // namespace oura_charts
