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
      using expected_json = expected<std::string, oura_exception>;

      
      /// <summary>
      ///   constructor, takes an Auth object and a URL that should be used
      ///   to build paths for REST endpoints.
      /// </summary>
      /// <remarks>
      ///   if a log object is provided, debug logging will be sent to the specified
      ///   logger for the REST calls.
      /// </remarks>
      RestDataProvider(Auth auth, std::string base_url) : m_auth{ auth },
                                                          m_base_url{ base_url }
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
      //template <typename DataT>
      [[nodiscard]] expected_json getJsonDataSeries(std::string_view path,
                                                    utc_timestamp start,
                                                    utc_timestamp end,
                                                    detail::nullable_string next_token = {}) const noexcept
      {
         using namespace oura_charts::constants;
         using enum spdlog::level::level_enum;

         auto start_param = toIsoDateTime(start);
         auto end_param = toIsoDateTime(end);
         logging::info("RestDataProvider - Retrieving json data for series [{}] for date range {} - {}", path, start_param, end_param);
         cpr::Parameters params{
            { REST_PARAM_START_DATETIME, start_param },
            { REST_PARAM_END_DATETIME, end_param}
         };

         if (next_token)
         {
            logging::info("RestDataProvider - next_token parameter [{}] supplied, requesting paged data", *next_token);
            params.Add(cpr::Parameter{ REST_PARAM_NEXT_TOKEN, *next_token });
         }

         // Send the request to server and check that we get a valid response.
         auto exp_json = doRestGet(path, params);
         if (exp_json)
         {
            logging::trace("RestDataProvider - doRestGet() returned the following JSON:\r\n{}", glz::prettify_json(exp_json.value()));
         }
         else
         {
            logging::info("RestDataProvider - doRestGet() returned an error (see next log record for details)");
            logging::exception(exp_json.error());
         }
         return exp_json;
      }


      // The base URL that is used in combination with the 'path' parameter of the
      // getJson() methods to build the full URL for the REST endpoint of an object(s)
      std::string baseURL() const { return m_base_url; }

   private:
      Auth m_auth{};
      std::string m_base_url{};

      // Assembles the REST GET request and sends it to the server, returning any JSON
      // (or error information) that is received in response.
      template <typename... Ts>
      [[nodiscard]] expected_json doRestGet(std::string_view path, Ts... ts) const noexcept
      {
         cpr::Header header{
            { constants::REST_HEADER_XCLIENT, constants::REST_HEADER_XCLIENT_VALUE }
         };

         // Send the request to server and check that we get a valid response.
         cpr::Response response = cpr::Get(m_auth.getAuthorization(), pathToUrl(path), ts...);
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
      [[nodiscard]] cpr::Url pathToUrl(std::string_view relative_path) const
      {
         return { fmt::format("{}/{}", m_base_url, relative_path) };
      }

   };

} // namespace oura_charts

