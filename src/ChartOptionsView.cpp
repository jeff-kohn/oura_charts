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
   bool ChartOptionsView::OnCreate(wxDocument* doc, long flags)
   {
      // Associate the appropriate frame with this view.
      auto main_frame = wxGetApp().getMainFrame();
      assert(main_frame);

      SetFrame(main_frame);
      main_frame->getCanvas()->setView(this);

      // Make sure the document manager knows that this is the
      // current view.
      Activate(true);

      // Initialize the edit menu Undo and Redo items
      //doc->GetCommandProcessor()->SetEditMenu(m_frame->GetEditMenu());
      //doc->GetCommandProcessor()->Initialize();
      return true;
   }

   void ChartOptionsView::OnDraw([[maybe_unused]] wxDC* dc)
   {
   }

   void ChartOptionsView::OnUpdate([[maybe_unused]] wxView* , [[maybe_unused]] wxObject* )
   {
   }

   bool ChartOptionsView::OnClose([[maybe_unused]] bool deleteWindow)
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
