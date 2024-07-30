#include "ChartView.h"
#include <wx/log.h>

namespace oura_charts
{


   void ChartView::OnDraw([[maybe_unused]] wxDC* dc)
   {
   }

   void ChartView::OnUpdate([[maybe_unused]] wxView* , [[maybe_unused]] wxObject* )
   {
   }

   bool ChartView::OnClose([[maybe_unused]] bool deleteWindow)
   {
      if (!GetDocument()->Close())
      {
         return false;
      }
      SetFrame(nullptr);
      Activate(false);
      return true;
   }
   
}  // namespace oura_charts
