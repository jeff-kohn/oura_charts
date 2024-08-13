//---------------------------------------------------------------------------------------------------------------------
// ChartDocTemplate.h
//
// header file for the ChartDocTemplate class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "ChartDocument.h"
#include "ChartOptionsView.h"
#include "doc_view_fwd.h"


namespace oura_charts
{

   class ChartDocTemplate : public wxDocTemplate
   {
   public:
      ChartDocTemplate(wxDocManager* manager,
                       const wxString& description,
                       const wxString& filter,
                       const wxString& dir,
                       const wxString& extension,
                       const wxString& docTypeName,
                       const wxString& viewTypeName);

      ChartDocument* CreateDocument(const wxString& path, long flags = 0) override;
      ChartOptionsView* CreateView(wxDocument* doc, long flags = 0) override;
   };

} // namespace oura_charts
