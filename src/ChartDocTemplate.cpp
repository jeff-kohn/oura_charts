//---------------------------------------------------------------------------------------------------------------------
// ChartDocTemplate.cpp
//
// source file for the ChartDocTemplate class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "ChartDocTemplate.h"
#include "ChartDocument.h"
#include "ChartOptionsView.h"

namespace oura_charts
{

   ChartDocTemplate::ChartDocTemplate(wxDocManager* manager, const wxString& description,
                                      const wxString& filter, const wxString& dir,
                                      const wxString& extension, const wxString& docTypeName,
                                      const wxString& viewTypeName)
      : wxDocTemplate{ manager, description, filter, dir, extension, docTypeName, viewTypeName }
   {
   }

   ChartDocument* ChartDocTemplate::CreateDocument(const wxString& path, long flags)
   {
      auto doc = std::make_unique<ChartDocument>();
      InitDocument(doc.get(), path, flags);
      return doc.release(); // wx forces this shit on us.
   }


   ChartOptionsView* ChartDocTemplate::CreateView(wxDocument* doc, long flags)
   {
     auto view = std::make_unique<ChartOptionsView>();
     view->SetDocument(doc);

     if (!view->OnCreate(doc, flags))
        return nullptr;

     return view.release();
   }


}  // namespace oura_charts
