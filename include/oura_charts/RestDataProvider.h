//---------------------------------------------------------------------------------------------------------------------
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

   /// <summary>
   ///   Provides a common interface for retrieving data objects from a REST endpoint.  
   /// </summary>
   /// <remarks>
   template <AuthObject Auth>
   class RestDataProvider
   {
   public:
      // this is the type we return when retrieving data. Exected value is the requested json,
      // unexpected value is an exception describing what went wrong.
      using expected_json = expected<std::string, oura_exception>;

      /// <summary>
      ///   constructor, takes an Auth object and an URL that should be used
      ///   to build paths for REST endpoints.
      /// </summary>
      RestDataProvider(Auth auth, std::string base_url) : m_auth{ auth }, m_base_url{ base_url }
      {
      }

      /// <summary>
      /// Retrieve the JSON for a single object from the rest server.
      /// </summary>
      [[nodiscard]] expected_json getJsonObject(std::string_view path) const noexcept
      {
         cpr::Header header{ {constants::REST_HEADER_XCLIENT, constants::REST_HEADER_XCLIENT_VALUE} };
         return getJsonFromResponse(cpr::Get(m_auth.getAuthorization(), pathToUrl(path), std::move(header)));
      }


      /// <summary>
      ///   Retrieve a data series from a REST endpoint into a container of structs. The target container is passed as a parameter. The
      ///   next_token is used to indicate the request is to retrieve additional data from a previous quest if applicable. The expected
      ///   retrun value is a DataSeries<T> containing the requested data. The unexpected value is error information if the request was
      ///   unable to retrieve the requested data.
      /// </summary>
      template <typename DataT>
      [[nodiscard]] expected<detail::RestDataSeries<DataT>, oura_exception> getJsonDataSeries(std::string_view path,
                                                                                              timestamp_utc start,
                                                                                              timestamp_utc end,
                                                                                              detail::nullible_string next_token = {}) const noexcept
      {
         using namespace oura_charts::constants;

         cpr::Header header{
            { REST_HEADER_XCLIENT, REST_HEADER_XCLIENT_VALUE }
         };
         cpr::Parameters params{
            { REST_PARAM_START_DATETIME, toIsoDateString(start) },
            { REST_PARAM_END_DATETIME, toIsoDateString(end)}
         };

         if (next_token)
            params.Add(cpr::Parameter{ REST_PARAM_NEXT_TOKEN, std::string{ *next_token } });

         // Send the request to server and check that we get a valid response.
         auto response = cpr::Get(m_auth.getAuthorization(), pathToUrl(path), header, params);
         auto exp_json = getJsonFromResponse(response);
         if (!exp_json.has_value())
            return unexpected(exp_json.error());

         // Read the JSON into a struct. It should contain a 'data' element that is
         // an array of the requested objects, and a next_token value that indicates
         // if there is more data.
         std::string json_text{ exp_json.value() };
         typename detail::RestDataSeries<DataT> data{};
         auto pe = glz::read_json(data, json_text);
         if (pe)
            return unexpected{ oura_exception{static_cast<int64_t>(pe.ec), glz::format_error(pe, json_text),ErrorCategory::Parse } };
         else
            return data;
      }

      // The base URL that is used in combination with the 'path' parameter of the
      // getJson() methods to build the full URL for the REST endpoint of an object(s)
      std::string baseURL() const { return m_base_url; }

   private:
      Auth m_auth;
      std::string m_base_url;

      // extract the expected json (or unexepected error) from a REST response
      [[nodiscard]] expected_json getJsonFromResponse(const cpr::Response& response) const noexcept
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

