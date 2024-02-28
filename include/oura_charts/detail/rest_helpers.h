#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/RestAuth.h"
#include "oura_charts/detail/utility.h"
#include "oura_charts/concepts.h"
#include <string>



namespace oura_charts::detail
{

   /// <summary>
   ///   alias for expected<json, oura_exception>
   /// </summary>
   using expected_json = expected<std::string, oura_exception>;


   /// <summary>
   ///   This function will parse the text of a REST response to a JSON object after checking for errors.
   ///   If the response indicates success the JSON will be returned. If the response indicates an error,
   ///   an oura_exception will be thrown containing the error information.
   /// </summary>
   //[[nodiscard]] json getJsonFromResponse(const cpr::Response& response);


   /// <summary>
   ///   translates a nlohman::basic_json::exception to an oura_exception with error message that
   ///   indicates the type being parsed as well the error that occured.
   /// </summary>
   //template<typename T>
   //[[nodiscard]] oura_exception translateException(const json::exception& e, T*  = nullptr)
   //{
   //   return{ e.id,
   //           fmt::format("error parsing JSON for type {}: {}", typeid(T).name(), e.what()),
   //           ErrorCategory::Parse };
   //}


   /// <summary>
   ///   translates a nlohman::basic_json::exception to an oura_exception with error message that
   ///   indicates the context in which the json was being parsed as well the error that occured.
   /// </summary>
   /*template<StringViewCompatible T>
   [[nodiscard]] oura_exception translateException(const json::exception& e, T context)
   {
      std::string_view ctx(context);
      return{ e.id, fmt::format("error parsing JSON for {}: {}", ctx, e.what()), ErrorCategory::Parse };
   }*/


   /// <summary>
   ///   Construct a object from a JSON document without throwing. Expected return val is the
   ///   newly created class instance. Unexpected return value is any error that occurred
   ///   parsing the json or initializing the object.
   /// </summary>
   template<RestNoThrowConstructable RestObject>
   [[nodiscard]] expected<RestObject, oura_exception> constructFromJsonNoThrow(expected_json&& json)
   {
       // todo: debugging
      try
      {
         if (json.has_value())
            return RestObject::makeFromJson(json.value());
         else
            return unexpected{ json.error() };
      }
      catch (json::exception& e)
      {
         return unexpected{ oura_exception{ e.id,
                                                 fmt::format("JSON Parse error for type {}: {}", typeid(RestObject).name(), e.what()),
                                                 ErrorCategory::Parse } };
      }
   }


   /// <summary>
   ///   Checks REST response for valid json data and either constructs the requested object or
   ///   propagates any error as an exception.
   /// </summary>
   /// <remarks>
   ///   Types constructible by this method need to have a single-param ctor that accepts a JSON
   ///   object. json parse exceptions are caught and tranlated to oura_exception.
   /// </remarks>
   template<RestConstructable RestObject>
   [[nodiscard]] RestObject constructFromJson(expected_json&& json)
   {
      // todo: debugging
      try
      {
         if (json)
            return RestObject{ json };
         else
            throw json.error();
      }
      catch (json::exception& e)
      {
         throw oura_exception{ e.id,
                               fmt::format("JSON Parse error for type {}: {}", typeid(RestObject).name(), e.what()),
                               ErrorCategory::Parse };
      }
   }


   /// <summary>
   ///   Construct one or more objects from a JSON array, adding them to the specified
   ///   container. If the expected_json value does not contain an array of json elements,
   ///   an execption will be thrown. Othwerise, exceptions will not be thrown if invididual
   ///   objects cannot be parsed (those errors will be added to the container as std::unexpected).
   /// </summary>
   template<RestDataSet DataSet>
   void constructFromJsonArray(expected_json&& json, DataSet& dataset)
   {
      using ObjectType = typename DataSet::value_type::value_type;

      if (!json)
         throw json.error();

      for (auto j : json.value())
      {
         dataset.push_back(constructFromJsonNoThrow<ObjectType>(j));
      }
   }


   /// <summary>
   ///   retrieve data from the REST api and use it to construct an object of the requested type.
   /// </summary>
   /// <remarks>
   ///   This method will convert json::exception and std::exception to oura_exception before re-throwing
   /// </remarks>
   template<RestConstructable RestObject, typename... Ts>
   [[nodiscard]] RestObject getObjectFromRestEndpoint(Ts&& ...ts)
   {
      try
      {
         cpr::Url url{ pathToUrl(RestObject::REST_PATH) };
         cpr::Header header{ {constants::REST_HEADER_XCLIENT, constants::REST_HEADER_XCLIENT_VALUE} };

         auto response = cpr::Get(url, header, std::forward<Ts>(ts)...);
         return constructFromJson<RestObject>(getJsonFromResponse(response));
      }
      catch (oura_exception&)
      {
         throw;
      }
      catch (std::exception& e)
      {
         throw oura_exception{ e.what(), ErrorCategory::REST };
      }
   }


   /// <summary>
   ///   retrieve multiple data objects of a given type from the REST api and add them to
   ///   the specified container. Does not throw exceptions on parsing errors
   ///   for individual objects, but will throw oura_exception if the overall operation failed
   ///   (eg we couldn't get a valid response from the REST server for some reason).
   /// </summary>
   /// <remarks>
   ///   DataSet containers are required to store elements of type expected<RestContructible, oura_exception>
   ///   The expected value will be the object representing the rest data, the unexpected value
   ///   will contain exception info if the json for the object could not be parsed.
   /// </remarks>
   template<RestDataSet DataSet, typename... Ts>
   [[nodiscard]] DataSet getDataSetFromRestEndpoint(Ts&& ...ts)
   {
      using std::string;
      using namespace constants;
      try
      {
         // Our dataset contains expected<ObjectType, oura_exception>.
         using ObjectType = typename DataSet::value_type::value_type;

         cpr::Url url{ pathToUrl(ObjectType::REST_PATH) };
         cpr::Header header{ {REST_HEADER_XCLIENT, REST_HEADER_XCLIENT_VALUE} };
         cpr::Parameter next_token{ REST_PARAM_NEXT_TOKEN, string{} };
         auto dataset = DataSet{};
         do
         {
            auto response = next_token.value.empty() ? cpr::Get(url, header, std::forward<Ts>(ts)...)
                                                     : cpr::Get(url, header, cpr::Parameters{next_token}, std::forward<Ts>(ts)...);
            json json_data = getJsonFromResponse(response);
            constructFromJsonArray<DataSet>(json_data.at(JSON_KEY_DATA), dataset);

            if (json_data.contains(REST_PARAM_NEXT_TOKEN))
            {
               auto j = json_data[REST_PARAM_NEXT_TOKEN];
               next_token.value = j.is_null() ? string{} : j.get<string>();
            }
         }
         while (!next_token.value.empty());

         return dataset;
      }
      catch (oura_exception&)
      {
         throw;
      }
      catch (std::exception& e)
      {
         throw oura_exception{ e.what() };
      }
   }

} // namespace oura_charts::detail
