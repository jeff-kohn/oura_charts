#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/RestAuth.h"
#include <cpr/cpr.h>

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
      ///   constructor, takes and Auth object and an URL that should be used
      ///   to build paths for REST endpoints.
      /// </summary>
      RestDataProvider(Auth auth, std::string base_url)
         : m_auth{ auth }, m_base_url{ base_url }
      {
      }

      /// <summary>
      /// Retrieve the JSON for an object from the rest server.
      /// </summary>
      [[nodiscard]] expected_json getJsonObject(std::string_view path) const noexcept
      {
         cpr::Header header{ {constants::REST_HEADER_XCLIENT, constants::REST_HEADER_XCLIENT_VALUE} };
         cpr::Url url{ fmt::format("{}/{}", m_base_url, path) };
         return getJsonFromResponse(cpr::Get(m_auth.getAuthorization(), std::move(url), std::move(header)));
      }


      [[nodiscard]] expected_json getJsonDataSeries(std::string_view path) const noexcept
      {
         return unexpected{ oura_exception{"Not Implememented"} };
      }

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

      inline static cpr::Url pathToUrl(std::string_view relative_path)
      {
         return { fmt::format("{}/{}", m_base_url, relative_path) };
      }
   };

} // namespace oura_charts

