//---------------------------------------------------------------------------------------------------------------------
// ChartOptionsCanvas.h
//
// header file for the ChartOptionsCanvas class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "doc_view_fwd.h"
#include "generated/ChartOptionsBase.h"
#include "oura_charts/SchemaManager.h"

namespace oura_charts
{

   /// <summary>
   ///   class representing the window canvas for our doc/view. provides dialog interface for setting chart options,
   ///   lives as a panel inside the main frame window.
   /// </summary>
   class ChartOptionsCanvas : public ChartOptionsBase
   {
   public:
      explicit ChartOptionsCanvas(MainFrame* parent, detail::SchemaManager mgr);

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

      // doesn't really do anything since our canvas is actually a form
      void OnDraw(wxDC& dc);

   protected:
      void onAddClicked(wxCommandEvent& event) override;
      void onDeleteClicked(wxCommandEvent& event) override;
      void onDeleteUpdateUI(wxUpdateUIEvent& event) override;
      void onEditClicked(wxCommandEvent& event) override;
      void onEditUpdateUI(wxUpdateUIEvent& event) override;
      void onEndDateSelected(wxDateEvent& event) override;
      void onRunQuery(wxCommandEvent& event) override;
      void onRunQueryUpdateUI(wxUpdateUIEvent& event) override;
      void onStartDateSelected(wxDateEvent& event) override;

   private:
      const detail::SchemaManager m_schema{};
      ChartOptionsView* m_view{};
      [[maybe_unused]] MainFrame* m_parent_frame{};
   };

}
