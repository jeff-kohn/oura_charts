#pragma once

#include "ChartDocument.h"
//#include "ChartView.h"

namespace oura_charts
{

   class ChartDocument;
   class ChartOptionsView;

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
      //ChartOptionsView* CreateView(wxDocument* doc, long flags = 0) override;
   };

} // namespace oura_charts
