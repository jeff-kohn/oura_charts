#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/RestAuth.h"
#include "oura_charts/detail/utility.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <expected>


namespace oura_charts::detail
{
   using json = nlohmann::json;


   /// <summary>
   ///   This function will parse the text of a REST response to a JSON object after checking for errors. The
   ///   expected return value is the parsed JSON object, the unexpected value contains error information
   ///   in the event of failure.
   /// </summary>
   [[nodiscard]] std::expected<json, oura_exception> getJsonFromResponse(const cpr::Response& response);


   /// <summary>
   ///   translates a nlohman::basic_json::exception to an oura_exception with error message that
   ///   indicates the type being parsed as well the error that occured.
   /// </summary>
   template<typename T>
   [[nodiscard]] oura_exception translateException(const json::exception& e, T*  = nullptr)
   {
      return{ e.id,
              fmt::format("JSON Parse error for type {}: {}", typeid(T).name(), e.what()),
              ErrorCategory::JSON };
   }


   /// <summary>
   ///   Check REST response for valid json data and either construct the requested object or
   ///   propagate any error as an exception.
   /// </summary>
   /// <remarks>
   ///   Types constructible by this method need to have a single-param ctor that accepts a JSON
   ///   object. json parse exceptions are caught and tranlated to oura_exception.
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

   /// <summary>
   ///   retrieve data from the REST api and use it to construct an object of the requested type.
   /// </summary>
   /// <remarks>
   ///   This method will convert json::exception and std::exception to oura_exception before re-throwing
   /// </remarks>
   template<typename ObjectType, typename... Ts>
   [[nodiscard]] auto getObjectFromRestEndpoint(Ts&& ...ts)
   {
      try
      {
         cpr::Url url{ pathToUrl(ObjectType::REST_PATH) };
         cpr::Header header{ {constants::REST_HEADER_XCLIENT, constants::REST_HEADER_XCLIENT_VALUE} };

         auto response = cpr::Get(url, header, std::forward<Ts>(ts)...);
         return constructFromJson<ObjectType>(getJsonFromResponse(response));
      }
      catch (oura_exception&) {
         throw;
      }
      catch (std::exception& e){
         throw oura_exception{ e.what() };
      }
   }

} // namespace oura_charts::detail
