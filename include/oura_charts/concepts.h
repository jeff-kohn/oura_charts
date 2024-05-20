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
#include <chrono>

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
   ///   concept for an auth object that can be used for authenticating with a data provider (REST, DB, etc).
   /// </summary>
   template <typename T>
   concept AuthObject = requires (T t)
   {
      t.getAuthorization();
   };


   template <typename T>
   concept DataSeriesElement = requires (T t, T::StorageType data)
   {
      t = T{ std::move(data) };
   };


   /// <summary>
   ///   concept for a data provider than can retrieve JSON objects from some data source, such as
   ///   a REST endpoint, database, unit test mock, etc.
   /// </summary>
   template <typename Provider>
   concept DataProvider = requires (Provider dp, Provider::expected_json ej)
   {

      ej = dp.getJsonObject("");
      
      // Can't get this stupid fucking concept to work, this shit is so fucking unintuitive.
      //ej = dp.getJsonDataSeries(std::string_view{}, std::chrono::sys_seconds{}, std::chrono::sys_seconds{});
   };


   /// <summary>
   ///   concept for a class that can be constructed from JSON and knows the URL
   ///   endpoint to retrieve its data from.
   /// </summary>
   template<typename T>
   concept RestNoThrowConstructable = requires (T t)
   {
      T::REST_PATH;
      //retval  = T::makeFromJson(std::string{}); 
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
