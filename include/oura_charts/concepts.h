//---------------------------------------------------------------------------------------------------------------------
// concepts.h
//
// template concepts used in the Oura Charts project.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------


#pragma once

#include <concepts>
#include <string_view>
#include <fmt/format.h>

namespace oura_charts
{

   /// <summary>
   ///   concept requiring a template arge to be std::exception or derived from std::exception
   /// </summary>
   template<typename T>
   concept ExceptionDerived = std::derived_from<T, std::exception> || std::same_as<T, std::exception>;

   template<typename T>
   concept FormattedException = ExceptionDerived<T> && requires (T t)
   {
      fmt::format("{}", t);
   };

   /// <summary>
   ///   concept requiring a type to be string_view or convertible to string_view
   /// </summary>
   template <typename T>
   concept StringViewCompatible = std::convertible_to<T, std::string_view>;


   /// <summary>
   ///   concept requireing a type to support the needed functionality to read/write/store
   ///   a collection of DataT objects.
   /// </summary>
   template <typename Container>
   concept DataSeriesContainer = std::ranges::output_range<Container>
                              && std::ranges::input_range<Container>
                              && std::ranges::forward_range<Container>;


   /// <summary>
   ///   concept for an auth object that can be used for authenticating with a data provider (REST, DB, etc).
   /// </summary>
   template <typename T>
   concept AuthObject = requires (T t)
   {
      t.getAuthorization();
   };


   /// <summary>
   ///   concept for a data provider than can retrieve JSON objects from some data source, such as
   ///   a REST endpoint, database, unit test, etc.
   /// </summary>
   template <typename T>
   concept DataProvider = requires (T dp, typename T::expected_json ej)
   {
      ej = dp.getJsonObject("");
   };


   /*/// <summary>
   ///   concept for a class that is can be constructed from JSON and knows the URL
   ///   endpoint to retrieve its data from.
   /// </summary>
   template<typename T>
   concept RestConstructable = requires (T t)
   {
      T::REST_PATH;
      T::makeFromJson("a json string");
   };*/

   /// <summary>
   ///   concept for a class that is can be constructed from JSON and knows the URL
   ///   endpoint to retrieve its data from.
   /// </summary>
   template<typename T>
   concept RestNoThrowConstructable = requires (T t)
   {
      T::REST_PATH;
      //retval  = T::makeFromJson(std::string{}); fucking bullshit motherfucker
   };


   /// <summary>
   ///   concept for a class that can contain a sequence of rest-constructible
   ///   objects.
   /// </summary>
   template<typename Container>
   concept RestDataSet = std::default_initializable<Container>
      && std::ranges::range<Container>
      && RestNoThrowConstructable<typename Container::value_type::value_type>
      && requires (Container cont, Container::value_type val)
      {
         Container::value_type::value_type::REST_PATH;
         cont.push_back(val);
         cont.reserve(55);
      };

} // namespace oura_charts
