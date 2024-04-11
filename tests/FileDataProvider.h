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
      using expected_json = expected<std::string, oura_exception>;

      // Initialize a file based data provider with a folder path. If the path doesn't exist,
      // a filesytem_error will be thrown.
      FileDataProvider(fs::path data_folder = ".");

      /// <summary>
      /// Retrieve the JSON for a single object.
      /// </summary>
      [[nodiscard]] expected_json getJsonObject(std::string_view path) const noexcept;

      /// <summary>
      ///   Retrieve the json for a series/array of objects. Note that this provider doesn't
      ///   actually do any date filtering, it's just  going to return the contents of the
      ///   file that matches the path name. The start/end/next params are just for consistency
      ///   with the REST-based provider's interface.
      /// </summary>
      [[nodiscard]] expected_json getJsonDataSeries(std::string_view path,
                                                    timestamp_utc start,
                                                    timestamp_utc end,
                                                    detail::nullable_string = {}) const noexcept;
   private:
      fs::path m_data_folder{};

      expected_json doFileGet(fs::path filename) const noexcept;
   };

} // oura_charts::detail
