#pragma once

#include "oura_charts/chrono_helpers.h"

#include <wx/docview.h>
#include <iostream>


namespace oura_charts
{

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
