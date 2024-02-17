#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/http_status_codes.h"
#include <cpr/response.h>
#include <nlohmann/json.hpp>
#include <expected>


namespace oura_charts::detail
{
   using json = nlohmann::json;

   template<typename T>
   [[nodiscard]] oura_exception translateException(const json::exception& e, T* caller = nullptr)
   {
      return{ e.id,
              fmt::format("JSON Parse error for type {}: {}", typeid(T).name(), e.what()),
              ErrorCategory::JSON };
   }

   /// <summary>
   ///
   /// This function will parse the text of a REST response to a JSON object after checking for errors. The
   /// expected return value is the parsed JSON object, the unexpected value contains error information
   /// in the event of failure.
   /// 
   /// </summary>
   [[nodiscard]] std::expected<json, oura_exception> getJsonFromResponse(const cpr::Response& response);


   /// <summary>
   /// 
   /// Check REST response for valid json data and either construct the requested object or
   /// propagate any error as an exception.
   /// 
   /// </summary>
   /// <remarks>
   /// 
   /// Types constructible by this method need to have a single-param ctor that accepts a JSON
   /// object. json parse exceptions are caught and tranlated to oura_exception.
   /// 
   /// </remarks>
   template<typename ObjectType>
   [[nodiscard]] auto constructFromJson(std::expected<json, oura_exception>&& expected_json)
   {
      // todo: log json text for debugging
      try
      {
         if (expected_json)
            return ObjectType{ *expected_json };
         else
            throw expected_json.error();
      }
      catch (json::exception& e)
      {
         throw oura_exception{ e.id,
                               fmt::format("JSON Parse error for type {}: {}", typeid(ObjectType).name(), e.what()),
                               ErrorCategory::JSON };
      }
   }

} // namespace oura_charts::detail
