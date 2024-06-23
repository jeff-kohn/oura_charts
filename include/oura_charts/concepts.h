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
#include <ranges>

namespace oura_charts
{
   namespace rgs = std::ranges;

   /// <summary>
   ///   concept requiring a template arge to be std::exception or derived from std::exception
   /// </summary>
   template<typename T>
   concept ExceptionDerived = std::derived_from<T, std::exception> || std::same_as<std::remove_cvref<T>, std::exception>;

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
   ///   concept for a range/view over a map where both the key and value type are
   ///   compatible with std::string_view
   /// </summary>
   template <typename T>
   concept KeyValueRange = rgs::forward_range<T>
                         && StringViewCompatible<typename T::mapped_type>
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
   concept DataProvider = requires (Provider dp, Provider::JsonResult jr)
   {
      jr = dp.getJsonData("");
   };


} // namespace oura_charts
