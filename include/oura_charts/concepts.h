//---------------------------------------------------------------------------------------------------------------------
// concepts.h
//
// template concepts used in the Oura Charts project.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/detail/nullable_types.h"
#include <fmt/format.h>
#include <concepts>
#include <functional>
#include <ranges>
#include <string_view>

namespace oura_charts
{
   namespace rg = std::ranges;
   namespace vw = rg::views;


   /// <summary>
   ///   concept requiring a template arg to be std::exception or derived from std::exception
   /// </summary>
   template<typename T>
   concept ExceptionDerived = std::derived_from<T, std::exception> || std::same_as<std::remove_cvref<T>, std::exception>;


   /// <summary>
   ///   concept rquiring a template arg to be ExceptionDerived and additionally support formatted output.
   /// </summary>
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
   ///   concept requiring that a functor can be called with the given arguments. Since the
   ///   concept tests using std::invoke(), this will work for member functions, free
   ///   functions, any Callable really.
   /// </summary>
   template <typename A, typename... Args>
   concept InvocableFor = requires (A a, Args... args) { std::invoke(a, args...); };


   /// <summary>
   ///   concept for a range/view over a map where both the key and value type are
   ///   compatible with std::string_view
   /// </summary>
   template <typename T>
   concept KeyValueRange = rg::forward_range<T>
                         && StringViewCompatible<typename T::mapped_type>
                         && StringViewCompatible<typename T::key_type>;


   /// <summary>
   ///   concept requiring that the return type of a projection matches the key type of map.
   /// </summary>
   template<typename MapT, typename ProjectionT>
   concept CompatibleKeyProjection = std::invocable<ProjectionT, typename MapT::mapped_type> &&
                                  requires (MapT map, MapT::key_type key, MapT::mapped_type val, ProjectionT proj)
   {
      map.insert(typename MapT::value_type{ proj(val), val });
   };


   /// <summary>
   ///   concept for an auth object that can be used for authenticating with a data provider (REST, DB, etc).
   /// </summary>
   template <typename T>
   concept AuthObject = requires (T t)
   {
      t.getAuthorization();
   };


   /// <summary>
   ///   Concept for a type that is a range of data structs that can be used to initialize a DataSeries<> object.
   /// </summary>
   template <typename RangeT, typename ElementT>
   concept JsonStructRange = rg::sized_range<RangeT> &&
                             std::same_as<rg::range_rvalue_reference_t<RangeT>, typename ElementT::StorageType&&>;


   /// <summary>
   ///   concept for a data provider than can retrieve JSON data from some data source, such as
   ///   a REST endpoint, database, unit test mock, etc.
   /// </summary>
   template <typename Provider>
   concept DataProvider = requires (Provider dp, Provider::JsonResult jr)
   {
      jr = dp.getJsonData("");
   };


   /// <summary>
   ///   concept for type that can represent a null value (in other words, opt::option<>) 
   /// </summary>
   template <typename T>
   concept NullableType = std::same_as<T, std::optional<typename T::value_type>>;


   /// <summary>
   ///   concept for type that can represent a null value (in other words, opt::option<>) 
   /// </summary>
   template <typename T>
   concept NullableNumeric = NullableType<T>
      && (std::integral<typename T::value_type> || std::floating_point<typename T::value_type>)
      && !std::same_as<typename T::value_type, char>
      && !std::same_as<typename T::value_type, unsigned char>;


   /// <summary>
   ///   concept requiring that a type NOT be nullable
   /// </summary>
   template <typename T>
   concept NotNullable = not NullableType<T>;

} // namespace oura_charts
