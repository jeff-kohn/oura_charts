//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsCanvas.cpp
//
// source file for the ChartOptionsCanvas class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "ChartOptionsCanvas.h"  
#include "ChartOptionsView.h"
#include "MainFrame.h"

namespace oura_charts
{
   ChartOptionsCanvas::ChartOptionsCanvas(MainFrame* parent) : ChartOptionsBase(parent), m_parent_frame(parent)
   {
   }

   void ChartOptionsCanvas::OnDraw(wxDC& dc)
   {
      if (m_view)
         m_view->OnDraw(&dc);
   }
} // namespace oura_charts
