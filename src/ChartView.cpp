#include "ChartView.h"
#include <wx/log.h>

namespace oura_charts
{


   void ChartView::OnDraw(wxDC* )
   {
   }

   void ChartView::OnUpdate(wxView* , wxObject* )
   {
   }

   bool ChartView::OnClose(bool deleteWindow)
   {
      if (!GetDocument()->Close())
      {
         return false;
      }
      SetFrame(NULL);
      Activate(false);
      return true;
   }
}
