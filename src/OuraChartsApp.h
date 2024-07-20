#pragma once

#include <wx/app.h>


namespace oura_charts
{
   class OuraChartsApp : public wxApp
   {
   public:
      OuraChartsApp();

      bool OnInit() override;
   };

DECLARE_APP(OuraChartsApp);

}  // namespace oura_charts