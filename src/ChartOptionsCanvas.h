//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsCanvas.h
//
// header file for the ChartOptionsCanvas class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "forward_declarations.h"
#include "generated/ChartOptionsBase.h"


namespace oura_charts
{

   /// <summary>
   ///   class representing the window canvas for our doc/view. provides dialog interface for setting chart options,
   ///   lives as a panel inside the main frame window.
   /// </summary>
   class ChartOptionsCanvas : public ChartOptionsBase
   {
   public:
      explicit ChartOptionsCanvas(wxWindow* parent) : ChartOptionsBase(parent) {}

      /// <summary>
      ///   set the active view. since this is an SDI app, we always use the same
      ///   canvas and just swap out the active view.
      /// </summary>
      void setView(ChartOptionsView* view)
      {
         m_view = view;
      }


      /// <summary>
      ///   returns a (weak) ptr to the currently active view, if any (may be nullptr)
      /// </summary>
      ChartOptionsView* getView()
      {
         return m_view;
      }

   protected:
      void onAddClicked(wxCommandEvent& event) override { event.Skip(); }
      void onDeleteClicked(wxCommandEvent& event) override { event.Skip(); }
      void onDeleteUpdateUI(wxUpdateUIEvent& event) override { event.Skip(); }
      void onEditClicked(wxCommandEvent& event) override { event.Skip(); }
      void onEditUpdateUI(wxUpdateUIEvent& event) override { event.Skip(); }
      void onEndDateSelected(wxDateEvent& event) override { event.Skip(); }
      void onRunQuery(wxCommandEvent& event) override { event.Skip(); }
      void onRunQueryUpdateUI(wxUpdateUIEvent& event) override { event.Skip(); }
      void onStartDateSelected(wxDateEvent& event) override { event.Skip(); }

   private:
      ChartOptionsView* m_view{};
   };

}
