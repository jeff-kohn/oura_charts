//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsView.cpp
//
// source file for the ChartOptionsView class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "ChartOptionsView.h"
#include "ChartOptionsCanvas.h"
#include "MainFrame.h"
#include "OuraChartsApp.h"

#include <wx/log.h>
#include <wx/cmdproc.h>


namespace oura_charts
{
   bool ChartOptionsView::OnCreate([[maybe_unused]] wxDocument* doc, [[maybe_unused]] long flags)
   {
      // Associate the appropriate frame with this view.
      m_main_frame = wxGetApp().getMainFrame();
      assert(m_main_frame);
      if (!m_main_frame)
         return false;

      SetFrame(m_main_frame);
      m_main_frame->getCanvas()->setView(this);

      // Make sure the document manager knows that this is the
      // current view.
      Activate(true);

      // Initialize the edit menu Undo and Redo items
      //doc->GetCommandProcessor()->SetEditMenu(m_frame->GetEditMenu());
      //doc->GetCommandProcessor()->Initialize();
      return true;
   }


   void ChartOptionsView::OnUpdate([[maybe_unused]] wxView* , [[maybe_unused]] wxObject* )
   {
      if (m_main_frame and m_main_frame->getCanvas())
         m_main_frame->getCanvas()->Refresh();
   }


   bool ChartOptionsView::OnClose([[maybe_unused]] bool deleteWindow)
   {
      if (!GetDocument()->Close())
         return false;

      assert(m_main_frame);
      if (m_main_frame)
      {
         m_main_frame->getCanvas()->setView(nullptr);
         m_main_frame->SetTitle(wxGetApp().GetAppDisplayName());
      }

      SetFrame(nullptr);
      Activate(false);

      return true;
   }
   
}  // namespace oura_charts
