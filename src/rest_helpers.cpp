//#include "oura_charts/detail/rest_helpers.h"

namespace oura_charts::detail
{
   [[nodiscard]]
   json  getJsonFromResponse(const cpr::Response& response)
   {
      // todo: logging
      if (cpr::status::is_success(response.status_code))
      {
         try
         {
            return json::parse(response.text);
         }
         catch (json::exception& e)
         {
            throw translateException(e, constants::ERROR_CONTEXT_REST_RESPONSE);
         }
         catch (std::exception& e)
         {
            throw oura_exception{ std::string{e.what()}, ErrorCategory::REST };
         }
      }
      else if (response.error.code != cpr::ErrorCode::OK)
      {
         throw oura_exception{ response.error };
      }
      else
      {
         throw oura_exception{ static_cast<int64_t>(response.status_code), response.reason, ErrorCategory::REST };
      }
   }

} // namespace oura_charts::detail
