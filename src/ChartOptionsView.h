//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsView.h
//
// header file for the ChartOptionsView class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "doc_view_fwd.h"
#include <wx/docview.h>

namespace oura_charts
{

   /// <summary>
   ///   View class for the chart doc, handles configuring the query/chart options.
   /// </summary>
   class ChartOptionsView : public wxView
   {
   public:

      bool OnCreate(wxDocument* doc, long flags) override;
      void OnUpdate(wxView* sender, wxObject* hint = nullptr) override;
      bool OnClose(bool deleteWindow = true) override;

   private:
      MainFrame* m_main_frame{};
   };

} // namespace oura_charts
