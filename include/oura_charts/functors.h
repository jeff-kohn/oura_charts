//---------------------------------------------------------------------------------------------------------------------
// functors.cpp
//
// some functors and algorithm adapters used in the project
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include <concepts>

namespace oura_charts
{
   /// <summary>
   ///   Helper for accumulating an average value.
   /// </summary>
   template <typename SumT>
   struct AverageAccumulator
   {
      SumT sum{0};
      size_t count{0};

      template <typename T> requires std::convertible_to<T, SumT>
      AverageAccumulator& operator+=(T val)
      {
         sum += val;
         ++count;
         return *this;
      }

      double getAverage() const { return static_cast<double>(sum) / count; }
   };


}  // namespace oura_charts
