#include "oura_charts/detail/rest_helpers.h"

namespace oura_charts::detail
{
   [[nodiscard]] std::expected<json, oura_exception>  getJsonFromResponse(const cpr::Response& response)
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
         return std::unexpected{ oura_exception{ std::string{e.what()}, ErrorCategory::JSON } };
      }
   }

}
