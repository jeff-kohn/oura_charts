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
   ///   that take callables by value, or you'll end up with an empty result.
   /// </summary>
   /// </summary>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// <remarks>
   template <typename T>
   class MinCalc
   {

   public:
      using InputType = T;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<InputType>;

      void operator()(const InputType& val) noexcept
      {
          m_result = m_result.has_value() ? std::min(*m_result, val) : val;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };

      bool hasResult() const noexcept
      {
         return m_result.has_value();
      }

      const ResultType& result() const noexcept
      {
         return m_result;
      } 

   private:
      ResultType m_result{};
   };


   /// <summary>
   ///   accumulate a maximum value. make sure to pass std::ref() to algorithms
   ///   that take callables by value.
   /// </summary>
   /// </summary>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// <remarks>
   template <typename T>
   class MaxCalc
   {
   public:
      using InputType = T;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<InputType>;


      void operator()(const T& val) noexcept
      {
         m_result = m_result.has_value() ? std::max(*m_result, val) : val;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };

      bool hasResult() const noexcept
      {
         return m_result.has_value();
      }

      const ResultType& result() const noexcept
      {
         return m_result;
      }

   private:
      ResultType m_result{};
   };


   /// <summary>
   ///   Functor to calculate a sum.
   /// </summary>
   /// </summary>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// <remarks>
   template<typename ValueTypeT, typename ResultTypeT = ValueTypeT>
   class SumCalc
   {
   public:
      using InputType = ValueTypeT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<ResultTypeT>;


      void operator()(const InputType& val) noexcept
      {
         if (m_result.has_value())
            *m_result += val;
         else
            m_result = val;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };

      bool hasResult() const noexcept
      {
         return m_result.has_value();
      }

      const ResultType& result() const noexcept
      {
         return m_result;
      }

   private:
      ResultType m_result{};
   };



   /// <summary>
   ///   Functor to calculate an average.
   /// </summary>
   /// </summary>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// <remarks>
   template<typename InputTypeT, typename ResultTypeT = double>
   class AvgCalc
   {
   public:
      using InputType = InputTypeT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<ResultTypeT>;


      void operator()(const InputType& val) noexcept
      {
         m_sum(val);
         ++m_count;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };

      ResultType result() const noexcept
      {
         assert(m_count);
         if (m_sum.hasResult())
         {
            auto sum = m_sum.result().value();
            ResultTypeT result_val = static_cast<ResultTypeT>(sum) / m_count;
            return ResultType{ result_val };
         }
         else
         {
            return ResultType{};
         }
      }

      /// <summary>
      ///   returns the count of values used in the calculation, which does NOT
      ///   include any null values that were passed.
      /// </summary>
      /// <returns></returns>
      size_t count() const noexcept
      {
         return m_count;
      }

      // returns true if a non-null/empty value has been calculated. If this returns
      // false result() will return an empty std::optional<>
      bool hasResult() const noexcept
      {
         return m_count > 0;
      }

   private:
      SumCalc<InputType, ResultTypeT>  m_sum{};
      size_t  m_count{0};
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
