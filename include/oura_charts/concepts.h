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
#include <ranges>

namespace oura_charts
{
   namespace rgs = std::ranges;

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


   template <typename T>
   concept KeyValueRange = rgs::input_range<T> && StringViewCompatible<typename T::mapped_type>
                                               && StringViewCompatible<typename T::key_type>;

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
   ///   concept for a data provider than can retrieve JSON data from some data source, such as
   ///   a REST endpoint, database, unit test mock, etc.
   /// </summary>
   template <typename Provider>
   concept DataProvider = requires (Provider dp, Provider::expected_json ej)
   {

      ej = dp.getJsonObject("");
      
      // Can't get this stupid fucking concept to work, this shit is so fucking unintuitive.
      //ej = dp.getJsonDataSeries(std::string_view{}, std::chrono::sys_seconds{}, std::chrono::sys_seconds{});
   };


} // namespace oura_charts
