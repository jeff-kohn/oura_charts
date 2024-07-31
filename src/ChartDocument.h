#pragma once
#include "oura_charts/chrono_helpers.h"
#include <wx/docview.h>


namespace oura_charts
{

   class ChartDocument : public wxDocument
   {
   public:
      ChartDocument() = default;
      ~ChartDocument() override = default;


   private:
      //year_month_day m_from_date{};
      //year_month_day m_until_date{};
   };

} // namespace oura_charts
