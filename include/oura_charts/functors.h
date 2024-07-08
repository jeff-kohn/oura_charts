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
#include <cassert>

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
   ///   that take callables by value, or you'll end up with an empty value.
   /// </summary>
   /// <remarks>
   ///   note that if you never actually call operator(), the result() function
   ///   will return an empty/null optional<>
   /// <remarks>
   template <typename T>
   class MinCalc
   {

   public:
      void operator()(const T& val) noexcept
      {
          m_result = m_result.has_value() ? std::min(*m_result, val) : val;
      }

      const std::optional<T>& result() const noexcept
      {
         return m_result;
      }

      bool hasResult() const noexcept
      {
         return m_result.has_value();
      }

   private:
      std::optional<T> m_result{};
   };


   /// <summary>
   ///   MinCalc specializatin that can handle nullable types (std::optional)
   /// </summary>
   template <NullableNumeric T>
   class MinCalc<T>
   {
   public:
      void operator()(const T& val) noexcept
      {
         if (val.has_value())
            m_result = m_result.has_value() ? std::min(*m_result, *val) : *val;
      }

      const T& result() const noexcept
      {
         return m_result;
      }

      bool hasResult() const noexcept
      {
         return m_result.has_value();
      }

   private:
      T m_result{};
   };


   /// <summary>
   ///   accumulate a maximum value. make sure to pass std::ref() to algorithms
   ///   that take callables by value.
   /// </summary>
   template <typename T>
   class MaxCalc
   {
   public:
      using ResultType = T;

      void operator()(const T& val) noexcept
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
   ///   specialization of MaxCalc for nullable types (std::optional)
   /// </summary>
   template <NullableNumeric T>
   class MaxCalc<T>
   {
   public:
      void operator()(const T& val) noexcept
      {
         if (val.has_value())
            m_result = m_result.has_value() ? std::max(*m_result, *val) : *val;
      }

      const T& result() const noexcept
      {
         return m_result;
      }

      bool hasResult() const noexcept
      {
         return m_result.has_value();
      }

   private:
      T m_result{};
   };


   /// <summary>
   ///   Functor to calculate a sum.
   /// </summary>
   /// <remarks>
   ///   if you get a compiler error about constraint not met, it means you're using a nullable type and
   ///   forgot to specify the result type (which is not specified in the specialization for nullable types)
   /// <remarks>
   template<typename ValueTypeT, typename ResultTypeT>
   class SumCalc
   {
   public:
      void operator()(const ValueTypeT& val) noexcept
      {
         m_sum += val;
      }

      ResultTypeT result() const noexcept
      {
         return m_sum;
      }

   private:
      ResultTypeT m_sum{};
   };


   /// <summary>
   ///   SumCalc specialization for nullable types. Unfortunately we can't provide default for
   ///   ResultTypeT in a partial specialization
   /// </summary>
   template<NullableNumeric ValueTypeT, NullableNumeric ResultTypeT>
   class SumCalc<ValueTypeT, ResultTypeT>
   {
   public:
      void operator()(const ValueTypeT& val) noexcept
      {
         if (val.has_value())
            m_sum += *val;
      }

      ResultTypeT result() const noexcept
      {
         return m_sum;
      }

   private:
      ResultTypeT m_sum{};
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
         assert(m_count);
         return static_cast<ResultType>(m_sum.result()) / m_count;
      }

      size_t count()const noexcept
      {
         return m_count;
      }

   private:
      SumCalc<T, ResultTypeT>  m_sum{};
      size_t      m_count{};
   };





   //
   // Helper template for deducing return type of a class method.
   //
   template<typename T>
   struct MethodReturnType;

   template<typename R, typename C, typename... Args>
   struct MethodReturnType<R(C::*)(Args...)> {
       using type = R;
   };

   template<typename MethodT>
   using MethodReturnType_t = typename MethodReturnType<MethodT>::type;


}  // namespace oura_charts
