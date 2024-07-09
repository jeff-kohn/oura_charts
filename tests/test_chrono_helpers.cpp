//---------------------------------------------------------------------------------------------------------------------
// test_chrono_helpers.cpp
//
// unit tests for chrono_helpers.h
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------


#include "oura_charts/chrono_helpers.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   using namespace std::literals;

   namespace
   {

      // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
      constexpr year_month_day ymd{ 2024y / 2 / 1 };
      constexpr hh_mm_ss tod_utc{ 12h + 3min + 33s };
      constexpr hh_mm_ss tod_local{ 6h + 3min + 33s };
      // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

      // NOLINTBEGIN(cert-err58-cpp)
      const sys_seconds sys_secs       = sys_days{ ymd } + tod_utc.to_duration();
      const local_seconds local_secs   = local_days{ ymd } + tod_local.to_duration();
      // NOLINTEND(cert-err58-cpp)

      static constexpr auto datetime_str          = "2024-02-01T12:03:33Z"sv;
      static constexpr auto date_str              = "2024-02-01"sv;
      static constexpr auto local_datetime_str    = "2024-02-01T06:03:33-6:00"sv;
      static constexpr auto tz_str                = "America/Chicago"sv;

   }  // namespace

   TEST_CASE("test_parseIsoDateTime", "[datetime][parsing]")
   {
      REQUIRE(sys_secs == parseIsoDateTime(local_datetime_str));
      REQUIRE(sys_secs == parseIsoDateTime(datetime_str));
   }


   TEST_CASE("test_parseIsoDate", "[datetime][parsing]")
   {
      REQUIRE(ymd == parseIsoDate(date_str));
   }


   TEST_CASE("test_toIsoDateTime", "[datetime][parsing]")
   {
      REQUIRE(datetime_str == toIsoDateTime(sys_secs));
   }


   TEST_CASE("test_toIsoDate", "[datetime][parsing]")
   {
      REQUIRE(date_str == toIsoDate(sys_days{ ymd }));
   }


   TEST_CASE("test_getCivilTime", "[datetime]")
   {
      auto [cal_date, civil_time] = getCivilTime(sys_secs);
      REQUIRE(cal_date == ymd);
      REQUIRE(civil_time.to_duration() == tod_utc.to_duration());
   }


   TEST_CASE("test_stripTimeOfDay", "[datetime]")
   {
      REQUIRE(static_cast<sys_seconds>(sys_days{ ymd }) == stripTimeOfDay(sys_secs));
   }


   TEST_CASE("test_localToUtc", "[datetime]")
   {
      auto& db = std::chrono::get_tzdb();
      const auto* tz = db.locate_zone(tz_str);
      REQUIRE(nullptr != tz);
      REQUIRE(local_secs == utcToLocal(sys_secs, tz) );
   }


   TEST_CASE("test_utcToLocal", "[datetime]")
   {
      auto& db = std::chrono::get_tzdb();
      const auto* tz = db.locate_zone(tz_str);
      REQUIRE(nullptr != tz);
      REQUIRE(sys_secs == localToUtc(local_secs, tz));
   }


} // namespace oura_charts::test
