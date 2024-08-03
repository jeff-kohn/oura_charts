#pragma once

#include "constants.h"
#include "oura_charts/TokenAuth.h"

#include <wx/app.h>
#include <wx/confbase.h>
#include <wx/docview.h>

#include <memory>

namespace oura_charts
{
   class OuraChartsApp : public wxApp
   {
   public:
      OuraChartsApp();

      bool OnInit() override;
      int OnExit() override;

      /// <summary>
      ///   Get the current config object. Calling this will throw an exception
      ///   instead of returning nullptr if there's no default config.
      /// </summary>
      wxConfigBase& getConfig() noexcept(false);
      const wxConfigBase& getConfig() const noexcept(false);

      /// <summary>
      ///   Gets the REST auth token to use for calls to Oura REST API. Returned
      ///   value may contain an error if no token could be retrieved.
      /// </summary>
      using TokenResult = expected<TokenAuth, oura_exception>;
      TokenResult getRestToken() const;

      /// <summary>
      ///   Get the document manager for the application.
      /// </summary>
      using DocManagerWeakPtr = std::weak_ptr<wxDocManager>;
      DocManagerWeakPtr getDocManager()
      {
         return m_doc_mgr;
      }

   private:
      std::shared_ptr<wxDocManager> m_doc_mgr{};
   };

}  // namespace oura_charts


wxDECLARE_APP(oura_charts::OuraChartsApp);
