//---------------------------------------------------------------------------------------------------------------------7
// RestDataProvider.h
//
// Declaration for class RestDataProvider<> which retrieves data from a REST HTTPs server.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/rest_auth.h"
#include "oura_charts/detail/logging.h"
#include "oura_charts/detail/json_structs.h"
#include "oura_charts/datetime_helpers.h"
#include <cpr/cpr.h>
#include <glaze/glaze.hpp>

namespace oura_charts
{
   namespace detail
   {
      /// <summary>
      ///   class used for logging callback from cpr.
      /// </summary>
      struct LoggingCallback
      {
         logging::log_ptr_t m_logger;

         void operator()(cpr::DebugCallback::InfoType type, std::string data, intptr_t userdata);
      };
   }

   /// <summary>
   ///   Provides a common interface for retrieving data objects from a REST endpoint.  
   /// </summary>
   template <AuthObject Auth>
   class RestDataProvider
   {
   public:
      // this is the type we return when retrieving data. Exected value is the requested json,
      // unexpected value is an exception describing what went wrong.
      using expected_json = expected<std::string, oura_exception>;

      
      /// <summary>
      ///   constructor, takes an Auth object and a URL that should be used
      ///   to build paths for REST endpoints.
      /// </summary>
      /// <remarks>
      ///   if a log object is provided, debug logging will be sent to the specified
      ///   logger for the REST calls.
      /// </remarks>
      RestDataProvider(Auth auth, std::string base_url, logging::log_ptr_t logger = nullptr)
         : m_auth{ auth },
           m_base_url{ base_url },
           m_logger{ std::move(logger) }
      {
      }

      /// <summary>
      ///   Retrieve the JSON for a single object from the rest server.
      /// </summary>
      [[nodiscard]] expected_json getJsonObject(std::string_view path) const noexcept
      {
         return doRestGet(path);
      }

       // <summary>
       //   Retrieve the JSON for a single object from the rest server.
       // </summary>
      template <typename... Ts>
      [[nodiscard]] expected_json getJsonObject(std::string_view path, Ts... ts) const noexcept
      {
         return doRestGet(path, ts...);
      }


      /// <summary>
      ///   Retrieve a data series from a REST endpoint into a container of structs. The next_token is used to
      ///   indicate the request is to retrieve additional data from a previous request if applicable.
      ///   The unexpected value is error information if the request was unable to retrieve the requested data.
      /// </summary>
      template <typename DataT>
      [[nodiscard]] expected_json getJsonDataSeries(std::string_view path,
                                                    timestamp_utc start,
                                                    timestamp_utc end, 
                                                    detail::nullable_string next_token = {}) const noexcept
      {
         using namespace oura_charts::constants;

         cpr::Parameters params{
            { REST_PARAM_START_DATETIME, toIsoDateTime(start) },
            { REST_PARAM_END_DATETIME, toIsoDateTime(end)}
         };

         if (next_token)
            params.Add(cpr::Parameter{ REST_PARAM_NEXT_TOKEN, *next_token });

         // Send the request to server and check that we get a valid response.
         return doRestGet(path, params);
      }


      // The base URL that is used in combination with the 'path' parameter of the
      // getJson() methods to build the full URL for the REST endpoint of an object(s)
      std::string baseURL() const { return m_base_url; }

   private:
      Auth m_auth{};
      std::string m_base_url{};
      logging::log_ptr_t m_logger{};

      // Assembles the REST GET request and sends it to the server, returning any JSON
      // (or error information) that is received in response.
      template <typename... Ts>
      expected_json doRestGet(std::string_view path, Ts... ts) const noexcept
      {
         cpr::Header header{
            { constants::REST_HEADER_XCLIENT, constants::REST_HEADER_XCLIENT_VALUE }
         };

         // Send the request to server and check that we get a valid response.
         auto response = m_logger ? cpr::Get(cpr::DebugCallback{ detail::LoggingCallback{m_logger} }, m_auth.getAuthorization(), pathToUrl(path), ts...)
                                  : cpr::Get(m_auth.getAuthorization(), pathToUrl(path), ts...);
         
         return getJsonFromResponse(response);
      }

      // extract the expected json (or unexepected error) from a REST response
      [[nodiscard]] expected_json getJsonFromResponse(const cpr::Response & response) const noexcept
      {
         if (cpr::status::is_success(response.status_code))
            return response.text;
         else if (response.error.code != cpr::ErrorCode::OK)
            return unexpected{ oura_exception{ response.error } };
         else
            return unexpected{ oura_exception{static_cast<int64_t>(response.status_code), response.reason, ErrorCategory::REST} };
      }

      // concatenate path with base url to get FQ URL
      cpr::Url pathToUrl(std::string_view relative_path) const
      {
         return { fmt::format("{}/{}", m_base_url, relative_path) };
      }
   };

} // namespace oura_charts

