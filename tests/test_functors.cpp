//---------------------------------------------------------------------------------------------------------------------
// test_functors.cpp
//
// unit tests for some fucnctors/aggregates
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "oura_charts/functors.h"
#include "oura_charts/datetime_helpers.h"
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <ranges>
#include <algorithm>
#include <random>


// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
namespace oura_charts::test
{
   namespace rg = std::ranges;
   namespace vw = rg::views;
   using namespace std::literals;

   namespace
   {
      constexpr year_month_day min_date{ 2024y / 1 / 1 };
      constexpr year_month_day max_date{ 2025y / 21 / 22 };

      constexpr auto min_tp = civilTimeToClockTime(min_date);
      constexpr auto max_tp = civilTimeToClockTime(max_date);

      constexpr std::array date_range = { min_date + months{3},
                                          max_date,
                                          min_date - months{2} + years{2},
                                          min_date };

      constexpr std::array int_range = { 5, 4, 1, 2, 3 };

      constexpr std::array double_range = { 5.1, 6.66, 0.3, 4.01 };

   } // namespace


   // helper function that uses our functors on a range of various element types to
   // find the desired values. Note, some of these functors return an optional<>
   template< template<typename T> class FuncT, rg::forward_range RangeT>
   auto testFunctor(RangeT&& rg) 
   {
      FuncT<rg::range_value_t<RangeT>> min_fn{};
      rg::for_each(rg, std::ref(min_fn));
      return min_fn.result();
   }


   TEST_CASE("test_MinCalc_int")
   {
      REQUIRE( testFunctor<MinCalc>(int_range).value() == 1 );
   }
   

   TEST_CASE("test_MinCalc_double")
   {
      REQUIRE( testFunctor<MinCalc>(double_range).value() == 0.3 );
   }


   TEST_CASE("test_MinCalc_chrono_types")
   {
      // test finding minimum ymd
      REQUIRE( testFunctor<MinCalc>(date_range).value() == min_date );

      // test finding min time_point
      auto&& tp_view = date_range | vw::transform([] (auto&& ymd) -> auto { return civilTimeToClockTime(ymd); });
      REQUIRE( min_tp == testFunctor<MinCalc>(tp_view).value());

      // test finding min duration.
      auto&& dur_view = tp_view | vw::transform([] (auto&& tp) -> auto { return getTimeOfDayDuration(tp); });
      REQUIRE( getCivilTime(min_tp).second.to_duration() == testFunctor<MinCalc>(dur_view).value());
   }


   TEST_CASE("test_MaxCalc_int")
   {
      REQUIRE(testFunctor<MaxCalc>(int_range).value() == 5);
   }


   TEST_CASE("test_MaxCalc_double")
   {
      REQUIRE(testFunctor<MaxCalc>(double_range).value() == 6.66);
   }


   TEST_CASE("test_MaxCalc_chrono_types")
   {
      // ymd
      REQUIRE(testFunctor<MaxCalc>(date_range).value() == max_date);

      // time_point
      auto&& tp_view = date_range | vw::transform([] (auto&& ymd) -> auto { return civilTimeToClockTime(ymd); });
      REQUIRE(max_tp == testFunctor<MaxCalc>(tp_view).value());

      // duration.
      auto&& dur_view = tp_view | vw::transform([] (auto&& tp) -> auto { return getTimeOfDayDuration(tp); });
      REQUIRE(getCivilTime(max_tp).second.to_duration() == testFunctor<MaxCalc>(dur_view).value());
   }

   TEST_CASE("test_SumCalc_AvgCalc_int")
   {
      auto sum = std::accumulate(int_range.begin(), int_range.end(), 0);
      auto avg = static_cast<double>(sum) / int_range.size();

      REQUIRE( sum == testFunctor<SumCalc>(int_range) );
      REQUIRE( avg == testFunctor<AvgCalc>(int_range) );
   }


   TEST_CASE("test_SumCalc_AvgCalc_double")
   {
      auto sum = std::accumulate(double_range.begin(), double_range.end(), 0.0);
      auto avg = sum / double_range.size();

      REQUIRE(sum == testFunctor<SumCalc>(double_range));
      REQUIRE(avg == testFunctor<AvgCalc>(double_range));
   }


}  // namespace oura_charts::test

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
