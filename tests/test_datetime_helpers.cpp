//---------------------------------------------------------------------------------------------------------------------
// test_datetime_helpers.h
//
// unit tests for datetime_helpers.h
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------


#include "oura_charts/oura_charts.h"
#include "oura_charts/datetime_helpers.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   using namespace std::literals::chrono_literals;

   TEST_CASE("parseIsoDateTime", "[datetime][parsing]")
   {
      chrono::year_month_day ymd{ 2024y / 2 / 1 };
      chrono::hh_mm_ss tod{ 12h + 3min + 33s };
      sys_seconds tp = chrono::sys_days{ ymd } + tod.to_duration();

      std::string datetime_str{ "2024-02-01T12:03:33Z" };
      std::string date_str{ "2024-02-01" };
      std::string local_datetime_str{ "2024-02-01T07:03:33-5:00" };

      REQUIRE(tp == parseIsoDateTime(local_datetime_str));
      REQUIRE(tp == parseIsoDateTime(datetime_str));
      REQUIRE(ymd == parseIsoDate(date_str));
      REQUIRE(date_str == toIsoDate(chrono::sys_days{ ymd }));
      REQUIRE(datetime_str == toIsoDateTime(tp));


   }


} // namespace oura_charts::test
