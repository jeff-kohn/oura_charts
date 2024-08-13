//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsCanvas.cpp
//
// source file for the ChartOptionsCanvas class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "ChartOptionsCanvas.h"  
#include "ChartOptionsView.h"


namespace oura_charts
{

   // Define the repainting behavior
   void ChartOptionsCanvas::OnDraw(wxDC& dc)
   {
      if (m_view)
         m_view->OnDraw(&dc);
   }
} // namespace oura_charts
