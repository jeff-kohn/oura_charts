//---------------------------------------------------------------------------------------------------------------------
// test_functors.cpp
//
// unit tests for some fucnctors/aggregates
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "oura_charts/functors.h"
#include "oura_charts/chrono_helpers.h"
#include <algorithm>
#include <array>
#include <catch2/catch_test_macros.hpp>
#include <ranges>

namespace oura_charts::test
{
   namespace rg = std::ranges;
   namespace vw = rg::views;
   using namespace std::literals;
   using nullable_int = std::optional<int>;

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

      constexpr std::array nullable_range = { nullable_int{}, nullable_int{5}, nullable_int{4}, nullable_int{1}, nullable_int{2}, nullable_int{3}, nullable_int{} };

   } // namespace


   // These functors will never have empty optional<> result because we never pass empty ranges to them. Any
   // invalid access to an empty optional would be a bug requiring a fix to the test, so we WANT that to throw.
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access) 


   // helper function that uses our functors on a range of various element types to
   // find the desired values. Note, some of these functors return an optional<>
   auto testFunctor(rg::forward_range auto const& rg, auto&& fn) 
   {
      rg::for_each(rg, std::ref(fn));
      return fn.result();
   }


   TEST_CASE("test_MinCalc_int")
   {
      auto val = testFunctor(int_range, MinCalc<int>{}).value();
      REQUIRE( val == 1 );
   }
   

   TEST_CASE("test_MinCalc_double")
   {
      REQUIRE( testFunctor(double_range, MinCalc<double>{}).value() == 0.3 );
   }


   TEST_CASE("test_MinCalc_nullable")
   {
      auto val = testFunctor(nullable_range, MinCalc<int>{}).value();
      REQUIRE(val == 1);
   }


   TEST_CASE("test_MinCalc_chrono_types")
   {
      // test finding minimum ymd
      REQUIRE( testFunctor(date_range, MinCalc<year_month_day>{}).value() == min_date );

      // test finding min time_point
      auto&& tp_view = date_range | vw::transform([] (auto&& ymd) -> auto { return civilTimeToClockTime(ymd); });
      REQUIRE( min_tp == testFunctor(tp_view, MinCalc<clock::time_point>{}).value());

      // test finding min duration.
      auto&& dur_view = tp_view | vw::transform([] (auto&& tp) -> auto { return getTimeOfDayDuration(tp); });
      REQUIRE( getCivilTime(min_tp).second.to_duration() == testFunctor(dur_view, MinCalc<clock::duration>{}).value());
   }


   TEST_CASE("test_MaxCalc_int")
   {
      REQUIRE(testFunctor(int_range, MaxCalc<int>{}).value() == 5);
   }


   TEST_CASE("test_MaxCalc_double")
   {
      REQUIRE(testFunctor(double_range, MaxCalc<double>{}).value() == 6.66);
   }


   TEST_CASE("test_MaxCalc_nullable")
   {
      REQUIRE(testFunctor(nullable_range, MaxCalc<int>{}).value() == 5);
   }


   TEST_CASE("test_MaxCalc_chrono_types")
   {
      // ymd
      REQUIRE(testFunctor(date_range, MaxCalc<year_month_day>{}).value() == max_date);

      // time_point
      auto&& tp_view = date_range | vw::transform([] (auto&& ymd) -> auto { return civilTimeToClockTime(ymd); });
      REQUIRE(max_tp == testFunctor(tp_view, MaxCalc<clock::time_point>{}).value());

      // duration.
      auto&& dur_view = tp_view | vw::transform([] (auto&& tp) -> auto { return getTimeOfDayDuration(tp); });
      REQUIRE(getCivilTime(max_tp).second.to_duration() == testFunctor(dur_view, MaxCalc<clock::duration>{}).value());
   }
   

   TEST_CASE("test_SumCalc_AvgCalc_int")
   {
      auto sum = std::accumulate(int_range.begin(), int_range.end(), 0);
      auto avg = static_cast<double>(sum) / int_range.size();

      REQUIRE( sum == testFunctor(int_range, SumCalc<int, int64_t>{}) );
      REQUIRE( avg == testFunctor(int_range, AvgCalc<int, double>{}) );
   }


   TEST_CASE("test_SumCalc_AvgCalc_double")
   {
      auto sum = std::accumulate(double_range.begin(), double_range.end(), 0.0);
      auto avg = sum / double_range.size();

      REQUIRE(sum == testFunctor(double_range, SumCalc<double>{}).value_or(0) );
      REQUIRE(avg == testFunctor(double_range, AvgCalc<double>{}).value_or(0) );
   }


   TEST_CASE("test_SumCalc_AvgCalc_nullable")
   {
      auto sum = std::accumulate(int_range.begin(), int_range.end(), 0);
      auto avg = static_cast<double>(sum) / int_range.size();

      REQUIRE(sum == testFunctor(nullable_range, SumCalc<int, uint64_t>{}));
      REQUIRE(avg == testFunctor(nullable_range, AvgCalc<int, double>{}));
   }

   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access)


}  // namespace oura_charts::test


