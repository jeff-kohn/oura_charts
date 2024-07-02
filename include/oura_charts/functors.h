//---------------------------------------------------------------------------------------------------------------------
// functors.h
//
// some functors and algorithm adapters used in the project
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include <functional>
#include <oura_charts/oura_charts.h>
#include <concepts>
#include <algorithm>
#include <optional>

namespace oura_charts
{
   //template <auto T>
   //constexpr auto projectionFunc = [] (auto&&... args) -> decltype(auto) { return std::invoke(T, std::forward<auto>(args)...); };


   /// <summary>
   ///   template that combines a sequence of lambda's into a functor that overloads
   ///   function operator() once for each lambda.
   /// </summary>
   /// <remarks>
   ///   useful for creating creating visitors from lambdas when working with variants, etc.
   /// </remarks>
   template < typename... Ts >
   struct overload : Ts...
   {
      using Ts:: operator()...;
   };
   template < typename... Ts > overload(Ts...) -> overload < Ts... >;


   /// <summary>
   ///   helper functor that can be used as a default predicate that always returns true.
   /// </summary>
   template<typename T>
   struct AlwaysTruePred
   {
      bool operator()(const T&) const noexcept
      {
         return true;
      }
   };


   /// <summary>
   ///   accumulate a minimum value. make sure to pass std::ref() to algorithms
   ///   that take callables by value.
   /// </summary>
   template <typename T>
   class MinCalc
   {
   public:
      using ResultType = T;

      template<typename ValT>
      void operator()(ValT&& val) noexcept
      {
          m_result = m_result.has_value() ? std::min(*m_result, val) : val;
      }

      const std::optional<ResultType>& result() const noexcept
      {
         return m_result;
      }

   private:
      std::optional<ResultType> m_result{};
   };


   /// <summary>
   ///   accumulate a minimum value. make sure to pass std::ref() to algorithms
   ///   that take callables by value.
   /// </summary>
   template <typename T>
   class MaxCalc
   {
   public:
      using ResultType = T;

      template<typename ValT>
      void operator()(ValT&& val) noexcept
      {
         m_result = m_result.has_value() ? std::max(*m_result, val) : val;
      }

      const std::optional<ResultType>& result() const noexcept
      {
         return m_result;
      }

   private:
      std::optional<ResultType> m_result{};
   };


   /// <summary>
   ///   Functor to calculate a sum.
   /// </summary>
   template<typename T, typename ResultTypeT = T>
   class SumCalc
   {
   public:
      using ResultType = ResultTypeT;

      void operator()(const T& val) noexcept
      {
         m_sum += val;
      }

      ResultType result() const noexcept
      {
         return m_sum;
      }

   private:
      ResultType m_sum{};
   };


   /// <summary>
   ///   Functor to calculate an average.
   /// </summary>
   template<typename T, typename ResultTypeT = double >
   class AvgCalc
   {
   public:
      using ResultType = ResultTypeT;

      void operator()(const T& val) noexcept
      {
         m_sum(val);
         ++m_count;
      }

      ResultType result() const noexcept
      {
         return static_cast<ResultType>(m_sum.result()) / m_count;
      }

      size_t count()const noexcept
      {
         return m_count;
      }

   private:
      SumCalc<T>  m_sum{};
      size_t      m_count{};
   };



}  // namespace oura_charts
