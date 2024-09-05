//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsCanvas.cpp
//
// source file for the ChartOptionsCanvas class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "ChartOptionsCanvas.h"

#include "AddDataFieldDialog.h"
#include "ChartOptionsView.h"
#include "MainFrame.h"

namespace oura_charts
{
   ChartOptionsCanvas::ChartOptionsCanvas(MainFrame* parent, detail::SchemaManager mgr) : ChartOptionsBase(parent),
                                                                                          m_schema(std::move(mgr)),
                                                                                          m_parent_frame(parent)
                                                                                          
   {
   }

   void ChartOptionsCanvas::OnDraw(wxDC& dc)
   {
      if (m_view)
         m_view->OnDraw(&dc);
   }

   void ChartOptionsCanvas::onAddClicked([[maybe_unused]] wxCommandEvent& event)
   {
      AddDataFieldDialog dlg(this, m_schema);
      dlg.ShowModal();
   }

   void ChartOptionsCanvas::onDeleteClicked(wxCommandEvent& event)
   {
      event.Skip();
   }

   void ChartOptionsCanvas::onDeleteUpdateUI(wxUpdateUIEvent& event)
   {
      event.Skip();
   }

   void ChartOptionsCanvas::onEditClicked(wxCommandEvent& event)
   {
      event.Skip();
   }

   void ChartOptionsCanvas::onEditUpdateUI(wxUpdateUIEvent& event)
   {
      event.Skip();
   }

   void ChartOptionsCanvas::onEndDateSelected(wxDateEvent& event)
   {
      event.Skip();
   }

   void ChartOptionsCanvas::onRunQuery(wxCommandEvent& event)
   {
      event.Skip();
   }

   void ChartOptionsCanvas::onRunQueryUpdateUI(wxUpdateUIEvent& event)
   {
      event.Skip();
   }

   void ChartOptionsCanvas::onStartDateSelected(wxDateEvent& event)
   {
      event.Skip();
   }
} // namespace oura_charts
