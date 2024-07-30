#include "ChartDocument.h"

namespace oura_charts
{

   void ChartDocument::OnDraw(wxDC* [[maybe_unused]] dc)
   {

   }


   void ChartDocument::OnUpdate(wxView* [[maybe_unused]] sender, wxObject* [[maybe_unused]] hint)
   {
   }


   bool ChartDocument::OnClose(bool [[maybe_unused]] deleteWindow)
   {
      return true;
   }


} // namespace oura_charts
