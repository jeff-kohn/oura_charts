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
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access)
   using namespace std::literals;

   namespace
   {
      constexpr year_month_day ymd{ 2024y / 2 / 1 };
      constexpr hh_mm_ss tod_utc{ 12h + 3min + 33s };
      constexpr hh_mm_ss tod_local{ 6h + 3min + 33s };

      // NOLINTBEGIN(cert-err58-cpp)
      const sys_seconds sys_secs       = sys_days{ ymd } + tod_utc.to_duration();
      const local_seconds local_secs   = local_days{ ymd } + tod_local.to_duration();
      // NOLINTEND(cert-err58-cpp)

      inline constexpr auto datetime_str          = "2024-02-01T12:03:33Z"sv;
      inline constexpr auto date_str              = "2024-02-01"sv;
      inline constexpr auto local_datetime_str    = "2024-02-01T06:03:33-6:00"sv;
      inline constexpr auto tz_str                = "America/Chicago"sv;

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

   TEST_CASE("test_AvgCal_duration")
   {
      using namespace std::chrono_literals;
      std::array sec_durations{ 20s, 30s, 40s, 50s };

      AvgCalc<seconds> avg_calc{};
      rg::for_each(sec_durations, std::ref(avg_calc));
      REQUIRE(avg_calc.hasResult());
      REQUIRE(avg_calc.count() == ssize(sec_durations));
      REQUIRE(avg_calc.result().value() == 35s);
   }

   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access) 

} // namespace oura_charts::test
