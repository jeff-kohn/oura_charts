#pragma once

#include <wx/docview.h>

namespace oura_charts
{

   class ChartOptionsView : public wxView
   {
   public:

   protected:
      bool OnCreate(wxDocument* doc, long flags) override;
      void OnDraw(wxDC* dc) override;
      void OnUpdate(wxView* sender, wxObject* hint = nullptr) override;
      bool OnClose(bool deleteWindow = true) override;

   private:
      
   };

} // namespace oura_charts
