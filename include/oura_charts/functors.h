//---------------------------------------------------------------------------------------------------------------------
// functors.h
//
// some functors and algorithm adapters used in the project
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include <oura_charts/oura_charts.h>
#include <concepts>
#include <algorithm>
#include <optional>

namespace oura_charts
{

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
   template<typename T, typename ResultTypeT = double>
   class AvgCalc
   {
   public:
      using ResultType = ResultTypeT;

      void operator()(const T& val) noexcept
      {
         m_sum += val;
         ++m_count;
      }

      ResultType result() const noexcept
      {
         return m_sum / m_count;
      }

   private:
      ResultType  m_sum{};
      size_t      m_count{};
   };



}  // namespace oura_charts
