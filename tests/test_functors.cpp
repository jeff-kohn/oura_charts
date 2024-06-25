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


namespace oura_charts::test
{
   namespace rgs = std::ranges;
   namespace vws = rgs::views;
   using namespace std::literals;


   template<rgs::forward_range RangeT>
   auto testMinCalc(RangeT&& rg) 
   {
      MinCalc<rgs::range_value_t<RangeT>> min_fn{};
      rgs::for_each(rg, std::ref(min_fn));
      return min_fn.result();
   }


   TEST_CASE("test_MinCalc_int")
   {
      std::array values = { 5, 4, 1, 2, 3 };
      REQUIRE(testMinCalc(values) == 1);
   }


   TEST_CASE("test_MinCalc_year_month_day")
   {

      std::array dates = { year_month_day{2024y / 3 / 1},
                           year_month_day{2024y / 1 / 1},
                           year_month_day{2024y / 1 / 6} };

      REQUIRE( testMinCalc(dates) == year_month_day{ 2024y / 1 / 1 } );
   }

}  // namespace oura_charts::test
