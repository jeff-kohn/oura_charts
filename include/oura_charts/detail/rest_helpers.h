#pragma once

#include "oura_charts/exception.h"
#include "oura_charts/detail/constants.h"
#include "oura_charts/detail/http_status_codes.h"
#include <cpr/response.h>
#include <nlohmann/json.hpp>
#include <expected>
#include <string>


namespace oura_charts::detail
{

   // convenience alias
   using json = nlohmann::json;

   /// <summary>
   ///
   /// This function will parse the text of a REST response to a JSON object after checking for errors. The
   /// expected return value is the parsed JSON object, the unexpected value will contain any error information
   /// in the event of failure.
   /// 
   /// </summary>
   std::expected<json, oura_exception> getJsonFromResponse(const cpr::Response& response, const std::string& context)
   {
      if (!cpr::status::is_success(response.status_code))
      {
         if (response.error.code != cpr::ErrorCode::OK)
         {
            return std::unexpected{ oura_exception{ response.error } };
         }
         else
         {
            // all we have is just an HTTP status code. use response text as error message if we have it,
            // or generic HTTP status text if not.
            auto&& text = response.text.empty() ? HttpStatus::reasonPhrase(response.status_code)
                                                : HttpStatus::reasonPhrase(response.status_code);

            return std::unexpected{ oura_exception{ static_cast<int64_t>(response.status_code),
                                                    std::string{ text },
                                                    ErrorCategory::REST } };
         }
      }

      try
      {
         return json::parse(response.text);
      }
      catch (std::exception& e)
      {
         return std::unexpected{ oura_exception{ std::string{e.what()}, ErrorCategory::JSON }};
      }
   }

} // namespace oura_charts::detail
