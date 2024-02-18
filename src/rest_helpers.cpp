#include "oura_charts/detail/rest_helpers.h"

namespace oura_charts::detail
{
   [[nodiscard]] std::expected<json, oura_exception>  getJsonFromResponse(const cpr::Response& response)
   {
      // todo: logging
      if (!cpr::status::is_success(response.status_code))
      {
         if (response.error.code != cpr::ErrorCode::OK)
            return std::unexpected{ oura_exception{ response.error } };
         else
            return std::unexpected{ oura_exception{ static_cast<int64_t>(response.status_code),
                                                    response.reason,
                                                    ErrorCategory::REST } };
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
