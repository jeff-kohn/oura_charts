//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsView.h
//
// header file for the ChartOptionsView class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include <wx/docview.h>

namespace oura_charts
{
   class MainFrame;

   class ChartOptionsView : public wxView
   {
   public:

      bool OnCreate(wxDocument* doc, long flags) override;
      void OnDraw(wxDC* dc) override;
      void OnUpdate(wxView* sender, wxObject* hint = nullptr) override;
      bool OnClose(bool deleteWindow = true) override;

   private:
      
   };

} // namespace oura_charts
