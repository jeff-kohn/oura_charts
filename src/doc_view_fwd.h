//---------------------------------------------------------------------------------------------------------------------
// doc_view_fwd.h
//
// header file for the ChartOptionsView class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include <memory>


namespace oura_charts
{
   // forward declare the main window class
   class MainFrame;

   // forward declare the doc class and pointers
   class ChartDocument;
   //using ChartDocumentPtr   = std::shared_ptr<ChartDocument>;
   //using ChartDocumentPtrWk = std::weak_ptr<ChartDocument>;

   // forward declare the view class and ptrs
   class ChartOptionsView;
   //using ChartOptionsViewPtr   = std::shared_ptr<ChartOptionsView>;
   //using ChartOptionsViewPtrWk = std::weak_ptr<ChartOptionsView>;

   // forward declare view window class and ptrs
   class ChartOptionsCanvas;
   //using ChartOptionsCanvasPtr   = std::shared_ptr<ChartOptionsCanvas>;
   //using ChartOptionsCanvasPtrWk = std::weak_ptr<ChartOptionsCanvas>;

   // forward declare the doc template and ptrs
   class ChartDocTemplate;
   //using ChartDocTemplatePtr   = std::shared_ptr<ChartDocTemplate>;
   //using ChartDocTemplatePtrWk = std::weak_ptr<ChartDocTemplate>;

}

class wxDocManager;
using DocManagerPtr   = std::shared_ptr<wxDocManager>;
using DocManagerPtrWk = std::weak_ptr<wxDocManager>;
