//---------------------------------------------------------------------------------------------------------------------
// OuraChartsApp.h
//
// header for the OuraChartsApp class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "constants.h"
#include "doc_view_fwd.h"
#include "oura_charts/TokenAuth.h"

#include <wx/app.h>
#include <wx/confbase.h>
#include <wx/docview.h>

#include <memory>
#include <filesystem>

namespace oura_charts
{
   namespace fs = std::filesystem;

   /// <summary>
   ///   app object for the OuraCharts applicatin.
   /// </summary>
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
      ///   Gets the main window for the application
      /// </summary>
      MainFrame* getMainFrame();

      /// <summary>
      ///   Get the document manager for the application.
      /// </summary>
      DocManagerPtrWk getDocManager();


      /// <summary>
      ///   Get location of where our schema files are located.
      /// </summary>
      /// <returns></returns>
      fs::path getSchemaFolder() const;

   private:
      DocManagerPtr m_doc_mgr{};
      MainFrame*    m_main_frame{};
   };

}  // namespace oura_charts


wxDECLARE_APP(oura_charts::OuraChartsApp);
