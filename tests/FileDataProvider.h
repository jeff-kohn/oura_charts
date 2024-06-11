//---------------------------------------------------------------------------------------------------------------------
// FileDataProvider.h
//
// Implements the Data Provider interface for testing, using json files on disk for the data rather than hitting a
// REST endpoint or database.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/datetime_helpers.h"
#include "oura_charts/detail/json_structs.h"
#include <string>
#include <filesystem>


namespace oura_charts::test
{
   namespace fs = std::filesystem;


   /// <summary>
   ///   Mock data provider used for unit tests for our JSON serialization. 
   /// </summary>
   class FileDataProvider
   {
   public:
      // this is the type we return when retrieving data for a single JSON object.
      // Exected value is the requested json, unexpected value is an exception
      // describing what went wrong in the event of failure.
      using JsonResult = expected<std::string, oura_exception>;

      // Initialize a file based data provider with a folder path. If the path doesn't exist,
      // a filesytem_error will be thrown.
      FileDataProvider(fs::path data_folder = ".");

      /// <summary>
      /// Retrieve the JSON for a single object (which for this will just be the contents
      /// of the file named 'path')
      /// </summary>
      [[nodiscard]] JsonResult getJsonObject(std::string_view path) const noexcept
      {
         return doFileGet(path);
      }

      /// <summary>
      ///   Retrieve the json for a series/array of objects. Note that this provider doesn't
      ///   actually do any date filtering, it's just  going to return the contents of the
      ///   file that matches the path name.
      /// </summary>
      /// <remarks>
      ///   the template parameter is just for interface compatibility, it's not actually used.
      /// </remarks>
      template <typename T>
      [[nodiscard]] JsonResult getJsonDataSeries(std::string_view path, T) const noexcept
      {
         return doFileGet(path);
      }

   private:
      fs::path m_data_folder{};

      JsonResult doFileGet(fs::path filename) const noexcept;
   };

} // oura_charts::detail
