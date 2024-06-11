//---------------------------------------------------------------------------------------------------------------------7
// RestDataProvider.h
//
// Declaration for class RestDataProvider<> which retrieves data from a REST HTTPs server.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/TokenAuth.h"
#include "oura_charts/detail/logging.h"
#include "oura_charts/detail/json_structs.h"
#include "oura_charts/datetime_helpers.h"
#include <cpr/cpr.h>
#include <optional>

namespace oura_charts
{

   /// <summary>
   ///   Provides a common interface for retrieving data objects from a REST endpoint.  
   /// </summary>
   /// <remarks>
   ///   this class uses std::expected<> instead of throwing exceptions, but doesn't guarantee
   ///   that exceptions won't be thrown further down the call chain.
   /// </remarks>
   template <AuthObject Auth>
   class RestDataProvider
   {
   public:
      // this is the type we return when retrieving data. Exected value is the requested json,
      // unexpected value is an exception describing what went wrong.
      using JsonResult = expected<std::string, oura_exception>;


      /// <summary>
      ///   Retrieve the JSON for a single object from the rest server.
      /// </summary>
      [[nodiscard]] JsonResult getJsonObject(std::string_view path) const noexcept
      {
         return doRestGet(path);
      }


      // <summary>
      //   Retrieve the JSON for a single object from the rest server, passing an arbitrary
      //   number of other objects to the underlying CPR call.
      // </summary>
      template<KeyValueRange MapT>
      [[nodiscard]] JsonResult getJsonObject(std::string_view path, const MapT&& param_map) const noexcept
      {
         return doRestGet(path, mapToParams(std::forward<MapT>(param_map)));
      }


      /// <summary>
      ///   Retrieve a collection of json data from the rest server
      /// </summary>
      /// <remarks>
      ///   path should be relative to base URL. param_map should contain the key/value
      ///   pairs that will be passed as parameters to the REST API. Be sure to include
      ///   next_token param as appropriate when requesting paged/chunked data.
      /// </remarks>
      template<KeyValueRange MapT>
      [[nodiscard]] JsonResult getJsonDataSeries(std::string_view path, MapT&& param_map) const noexcept
      {
         // Send the request to server and check that we get a valid response.
         return  doRestGet(path, mapToParams(std::forward<MapT>(param_map)));
      }


      // The base URL that is used in combination with the 'path' parameter of the
      // getJson() methods to build the full URL for the REST endpoint of an object(s)
      std::string baseURL() const { return m_base_url; }


      /// <summary>
      ///   constructor, takes an Auth object and a base URL that should be used
      ///   to build paths for REST endpoints.
      /// </summary>
      RestDataProvider(Auth auth, std::string base_url) : m_auth{ auth },  m_base_url{ base_url }
      {
      }

   private:
      Auth m_auth{};
      std::string m_base_url{};


      // Assembles the REST GET request and sends it to the server, returning any JSON
      // (or error information) that is received in response.
      template <typename... Ts>
      [[nodiscard]] JsonResult doRestGet(std::string_view path, Ts... ts) const noexcept
      {
         cpr::Header header{
            { constants::REST_HEADER_XCLIENT, constants::REST_HEADER_XCLIENT_VALUE }
         };

         // Send the request to server and check that we get a valid response.
         cpr::Response response = cpr::Get(m_auth.getAuthorization(), pathToUrl(path), ts...);
         return getJsonFromResponse(response);
      }


      // extract the expected json (or unexepected error) from a REST response
      [[nodiscard]] JsonResult getJsonFromResponse(const cpr::Response& response) const noexcept
      {
         if (cpr::status::is_success(response.status_code))
         {
            logging::trace("RestDataProvider - received the following JSON:\r\n{}", response.text);
            return response.text;
         }
         else if (response.error.code != cpr::ErrorCode::OK)
         {
            oura_exception ex{ response.error };
            logging::exception("RestDataProvider", ex);
            return unexpected{ std::move(ex) };
         }
         else
         {
            oura_exception ex{static_cast<int64_t>(response.status_code), response.reason, ErrorCategory::REST};
            logging::exception("RestDataProvider", ex);
            return unexpected{ ex };
         }
      }


      // concatenate path with base url to get FQ URL
      [[nodiscard]] cpr::Url pathToUrl(std::string_view relative_path) const
      {
         return { fmt::format("{}/{}", m_base_url, relative_path) };
      }


      template<KeyValueRange MapT>
      [[nodiscard]] static cpr::Parameters mapToParams(const MapT& param_map)
      {
         // get the parameters into object for the REST call.
         cpr::Parameters params{};
         for (auto&& elem : param_map)
         {
            logging::info("RestDataProvider - adding parameter '{}' = {}", elem.first, elem.second);
            params.Add({ elem.first, elem.second });
         };
         return params;
      }

   };

} // namespace oura_charts

