#include "ChartDocument.h"

namespace oura_charts
{

   void ChartDocument::OnDraw([[maybe_unused]]wxDC*  dc)
   {

   }


   void ChartDocument::OnUpdate([[maybe_unused]] wxView* sender, [[maybe_unused]] wxObject* hint)
   {
   }


   bool ChartDocument::OnClose([[maybe_unused]] bool deleteWindow)
   {
      return true;
   }


} // namespace oura_charts
