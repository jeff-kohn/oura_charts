#pragma once

#include <wx/docview.h>


namespace oura_charts
{

   class ChartDocument : public wxDocument
   {
   public:
      virtual void OnDraw(wxDC* dc);
      virtual void OnUpdate(wxView* sender, wxObject* hint = nullptr);
      virtual bool OnClose(bool deleteWindow = true);
   };

} // namespace oura_charts
