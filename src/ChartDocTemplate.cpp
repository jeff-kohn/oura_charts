#include "ChartDocTemplate.h"
#include <memory>


namespace oura_charts
{



   ChartDocTemplate::ChartDocTemplate(wxDocManager* manager, const wxString& description,
                                      const wxString& filter, const wxString& dir,
                                      const wxString& extension, const wxString& docTypeName,
                                      const wxString& viewTypeName)
      : wxDocTemplate{ manager, description, filter, dir, extension, docTypeName, viewTypeName }
   {
   }

   ChartDocument* oura_charts::ChartDocTemplate::CreateDocument(const wxString& path, long flags)
   {
      auto doc = std::make_unique<ChartDocument>();
      InitDocument(doc.get(), path, flags);
      return doc.release();
   }


   ChartView* ChartDocTemplate::CreateView(wxDocument* doc, long flags)
   {
     auto view = std::make_unique<ChartView>();
     view->SetDocument(doc);

     if (!view->OnCreate(doc, flags))
        return nullptr;

     return view.release();
   }


}
