//---------------------------------------------------------------------------------------------------------------------
// TestDataProvider.h
//
// Implements the Data Provider interface for testing, using json files on disk for the data rather than hitting a
// REST endpoint or database.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <variant>


namespace oura_charts::test
{
   namespace fs = std::filesystem;


   /// <summary>
   ///   Mock data provider used for unit tests for our JSON serialization. Can serve
   ///   files from a folder on disk, or from in-memory json strings.
   /// </summary>
   /// <remarks>
   ///   The "path" identifier used by this class is a lower-case version of the filename
   ///   minus extension for disk files; for in-memory strings it's the lower-case version
   ///   of whatever id is passed to addJsonData().
   /// </remarks>
   class TestDataProvider
   {
   public:
      // this is the type we return when retrieving data for a single JSON object.
      // Exected value is the requested json, unexpected value is an exception
      // describing what went wrong in the event of failure.
      using JsonResult = expected<std::string, oura_exception>;


      /// <summary>
      ///   Initialize a provider instance with json sources defined in files from a folder
      ///   on disk.
      /// </summary>
      /// <remarks>
      ///   The filename(s) will be used as the path id's when calling getXXX(). The file
      ///   contents will not be loaded into memory unless explicitly retrieved No caching
      ///   or filtering based on file extension/type is done.
      ///
      ///   If the specified data_folder doesn't exist or is empty, the object will be
      ///   empty-intialized, and won't be able to provide any JSON data unless you call
      ///   addJsonData() to add in-memory strings to the provider.
      /// </remarks>
      explicit TestDataProvider(const fs::path& data_folder = {});


      /// <summary>
      ///   Retrieve the JSON data associated with the specified path. expected return value
      ///   is the JSON text, unexpected is an oura_exception.
      /// </summary>
      [[nodiscard]] JsonResult getJsonData(std::string_view path) const noexcept
      {
         return getJsonData(path, "");
      }


      /// <summary>
      ///   Retrieve the JSON data associated with the specified path. expected return value
      ///   is the JSON text, unexpected is an oura_exception.
      /// </summary>
      /// <remarks>
      ///   currently the only MapT param this provider supports is "next_token". Others will be
      ///   ignored, and if you pass a non-empty MapT that doesn't contain "next_token" an exception
      ///   will be thrown.
      /// </remarks>
      template<KeyValueRange MapT>
      [[nodiscard]] JsonResult getJsonData(std::string_view path, const MapT&& param_map) const noexcept
      {
         // we may get called with empty MapT by code that doesn't know whether it's passing params or not.
         if (param_map.empty())
            return getJsonData(path);

         // pull the next_token value out, so we can use it to look up paged data.
         auto it = param_map.find(constants::REST_PARAM_NEXT_TOKEN);
         if (it == param_map.end())
            return unexpected{ oura_exception{ "TestDataProvider::getJsonData() called with invalid param_map, missing next_token", ErrorCategory::Parse } };

         return getJsonData(path, it->second);
      }


      /// <summary>
      ///   Add  the specified JSON text to the provider using the specified path as
      ///   its key.
      /// </summary>
      /// <returns>
      ///   true if a new object was added or an existing object was replaced, false if the
      ///   id already existing and overwrite_existing was 'false'
      /// </returns>
      bool addJsonData(std::string_view path, std::string_view json_data, bool overwrite_existing = false);
      bool addJsonData(std::string path, std::string json_data, std::string_view next_token, bool overwrite_existing = false);


      /// <summary>
      ///   turn an existing data source into a paged data source by duplicating it to multiple
      ///   sections. may throw on error.
      /// </summary>
      void paginateDataSource(std::string_view path, size_t num_pages) noexcept(false);

   private:
      using JsonEntry = std::variant<fs::path, std::string>;
      using JsonMap = std::map<std::string, JsonEntry, std::less<>>;

      JsonMap m_json_map{};

      void enumerateJsonFromFolder(const fs::path& data_folder);
      [[nodiscard]] JsonResult getJsonData(std::string_view path, std::string_view next_token) const noexcept;
      [[nodiscard]] JsonResult getJsonFile(const fs::path& json_path)  const noexcept;
   };

} // namespace oura_charts::test
