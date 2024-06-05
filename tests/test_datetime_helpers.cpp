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

   namespace
   {
      chrono::year_month_day ymd{ 2024y / 2 / 1 };
      chrono::hh_mm_ss tod{ 12h + 3min + 33s };
      sys_seconds tp = chrono::sys_days{ ymd } + tod.to_duration();

      std::string datetime_str{ "2024-02-01T12:03:33Z" };
      std::string date_str{ "2024-02-01" };
      std::string local_datetime_str{ "2024-02-01T07:03:33-5:00" };
   }

   TEST_CASE("test_parseIsoDateTime", "[datetime][parsing]")
   {
      REQUIRE(tp == parseIsoDateTime(local_datetime_str));
      REQUIRE(tp == parseIsoDateTime(datetime_str));
   }

   TEST_CASE("test_parseIsoDate", "[datetime][parsing]")
   {
      REQUIRE(ymd == parseIsoDate(date_str));
   }

   TEST_CASE("test_toIsoDateTime", "[datetime][parsing]")
   {
      REQUIRE(datetime_str == toIsoDateTime(tp));
   }

   TEST_CASE("test_toIsoDate", "[datetime][parsing]")
   {
      REQUIRE(date_str == toIsoDate(chrono::sys_days{ ymd }));
   }

   TEST_CASE("test_getCivilTime", "[datetime][parsing]")
   {
      auto [cal_date, civil_time] = getCivilTime(tp);
      REQUIRE(cal_date == ymd);
      REQUIRE(civil_time.to_duration() == tod.to_duration());
   }


} // namespace oura_charts::test
