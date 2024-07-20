#pragma once

#include "constants.h"
#include "oura_charts/TokenAuth.h"

#include <wx/app.h>
#include <wx/confbase.h>
#include <expected>


namespace oura_charts
{
   class OuraChartsApp : public wxApp
   {
   public:
      OuraChartsApp();

      bool OnInit() override;

      /// <summary>
      ///   Get the current config object. Calling this will throw an exception
      ///   instead of returning nullptr if there's no default config.
      /// </summary>
      wxConfigBase& getConfig() noexcept(false);
      const wxConfigBase& getConfig() const noexcept(false);

      using TokenResult = std::expected<TokenAuth, oura_exception>;
      TokenResult getRestToken() const;

   private:
   };

}  // namespace oura_charts


DECLARE_APP(oura_charts::OuraChartsApp);
