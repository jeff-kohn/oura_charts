//---------------------------------------------------------------------------------------------------------------------
// aggregate_functors.h
//
// Defines functors for aggregate query operators as well as factory method that can create functors corrsponding
// to an an enum value
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/chrono_helpers.h"

#include <variant>


namespace oura_charts::detail
{

   /// <summary>
   ///   functor that saves the first values passed to it
   ///   (ignoring all others)
   /// </summary>
   /// </summary>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// <remarks>
   template<typename InputTypeT>
   class FirstCalc
   {
   public:
      using InputType = InputTypeT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<InputType>;


      void operator()(const InputType& val) noexcept
      {
         if (!m_result)
            m_result = val;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };


      /// <summary>
      ///   return the result that was calculated. may be a null/empty optional if operator() was
      ///   never called with a non-empty value.
      /// </summary>
      const ResultType& result() const noexcept
      {
         return m_result;
      }

   private:
      ResultType m_result{};
   };


   /// <summary>
   ///   Functor that saves the last value passed to it.
   /// </summary>
   /// </summary>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// <remarks>
   template<typename InputTypeT>
   class LastCalc
   {
   public:
      using InputType = InputTypeT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<InputType>;


      void operator()(const InputType& val) noexcept
      {
         m_result = val;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };


      /// <summary>
      ///   return the result that was calculated. may be a null/empty optional if operator() was
      ///   never called with a non-empty value.
      /// </summary>
      const ResultType& result() const noexcept
      {
         return m_result;
      }

   private:
      ResultType m_result{};
   };


   /// <summary>
   ///   functor class that just returns a count of non-null objects
   ///   passed to it.
   /// </summary>
   /// </summary>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// <remarks>
   template<typename InputTypeT, typename ResultTypeT = int>
   class CountCalc
   {
   public:
      using InputType = InputTypeT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<ResultTypeT>;


      void operator()(const InputType&) noexcept
      {
         *m_result += 1;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };


      /// <summary>
      ///   return the result that was calculated. may be a null/empty optional if operator() was
      ///   never called with a non-empty value.
      /// </summary>
      ResultType result() const noexcept
      {
         return m_result;
      }

   private:
      ResultType m_result{};
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
   /// <remarks>
   ///   Since we support null input values, it's possible that the result could be null/empty; so
   ///   the default/initial return value is an empty std::optional<> instead of 0 until you pass in
   ///   at least one non-null value.
   /// </remarks>
   template<typename ValueTypeT, typename ResultTypeT = ValueTypeT>
   class SumCalc
   {
   public:
      using InputType = ValueTypeT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<ResultTypeT>;


      void operator()(const InputType& val) noexcept
      {
         // Cast is needed to prevent warnings when input type is integral and result type is floating point
         if (m_result.has_value())
            *m_result += static_cast<ResultTypeT>(val);
         else
            m_result = ResultType{ static_cast<ResultTypeT>(val) };
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };

      // returns the result of the calculation. may be be an empty/null value, so check before dereferencing
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


      /// <summary>
      ///   return the result that was calculated. may be a null/empty optional if operator() was
      ///   never called with a non-empty value.
      /// </summary>
      ResultType result() const noexcept
      {
         auto sum = m_sum.result();
         if (sum.has_value())
         {
            ResultTypeT result_val = static_cast<ResultTypeT>(sum.value()) / m_count;
            return ResultType{ result_val };
         }
         else
         {
            return ResultType{};
         }
      }


      /// <summary>
      ///   returns the count of values used in the calculation, which does NOT
      ///   include any null values that were passed in.
      /// </summary>
      size_t count() const noexcept
      {
         return m_count;
      }

   private:
      SumCalc<InputType, ResultTypeT>  m_sum{};
      size_t  m_count{ 0 };
   };


   /// <summary>
   ///   AvgCalc<> specialization for chrono duration types
   ///  
   ///   if you get compile error trying to pass this object type to an algorithm, it's because
   ///   this functor is move-only, you need to use std::ref() with algorithms that accept their
   ///   functor by-value.
   /// </summary>
   template<ChronoDuration DurationT>
   class AvgCalc<DurationT, double>
   {
   public:
      using Rep = DurationT::rep;
      using InputType = DurationT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<InputType>;

      void operator()(const InputType& val) noexcept
      {
         m_sum(val.count());
         ++m_count;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };


      /// <summary>
      ///   return the result that was calculated. may be a null/empty optional if operator() was
      ///   never called with a non-empty value.
      /// </summary>
      ResultType result() const noexcept
      {
         // Hitting this means you either averaged an empty/null-only range, or you forgot to
         // use std::ref() when passing the functor to a std:: algorithm.
         assert(m_count != 0);

         auto sum = m_sum.result();
         if (sum.has_value())
         {
            InputType result_val{ static_cast<Rep>(sum.value() / m_count) };
            return ResultType{ result_val };
         }
         else
         {
            return ResultType{};
         }
      }

      size_t count() const noexcept
      {
         return m_count;
      }

   private:
      SumCalc<Rep, double> m_sum{};
      size_t m_count{};
   };






   /// <summary>
   ///   variant type to contain one of a selection of aggregate functors
   /// </summary>
   template <typename... Types>
   using AggregrateFuncVt = std::variant< FirstCalc<Types>..., LastCalc<Types>..., CountCalc<Types>...,
                                          MinCalc<Types>..., MaxCalc<Types>..., SumCalc<Types>...,
                                          AvgCalc<Types>... >;



   /// <summary>
   ///   Enum for specifying an aggregate functor
   /// </summary>
   enum class AggregateSelection
   {
      First,
      Last,
      Min,
      Max,
      Count,
      Sum,
      Avg,
   };


   /// <summary>
   ///   Get an aggregate functor based on aggregate enum and input data type.
   /// </summary>
   template<typename VariantT, typename InputT>
   [[nodiscard]] constexpr VariantT getAggregateFunctor(AggregateSelection aggregate)
   {
      using enum AggregateSelection;
      switch (aggregate)
      {
      case First:
         return VariantT{ FirstCalc<InputT>{} };
      case Last:
         return VariantT{ LastCalc<InputT>{} };
      case Min:
         return VariantT{ MinCalc<InputT>{} };
      case Max:
         return VariantT{ MaxCalc<InputT>{} };
      case Count:
         return VariantT{ CountCalc<InputT>{} };
      case Sum:
         return VariantT{ SumCalc<InputT>{} };
      case Avg:
         return VariantT{ AvgCalc<InputT>{} };
      default:
         assert(false);
         std::unreachable();
      }
   };
} // namespace oura_charts::detail
