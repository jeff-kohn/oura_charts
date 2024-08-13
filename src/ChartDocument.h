//---------------------------------------------------------------------------------------------------------------------
// ChartDocument.h
//
// header file for the ChartDocument class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "forward_declarations.h"
#include "oura_charts/chrono_helpers.h"

#include <wx/docview.h>
#include <iostream>


namespace oura_charts
{
   /// <summary>
   ///  The document class for our application
   /// </summary>
   class ChartDocument : public wxDocument
   {
   public:
      ChartDocument() = default;
      ~ChartDocument() override = default;

      std::ostream& SaveObject(std::ostream& stream) override;
      std::istream& LoadObject(std::istream& stream) override;

   private:
      //year_month_day m_from_date{};
      //year_month_day m_until_date{};
   };

} // namespace oura_charts
