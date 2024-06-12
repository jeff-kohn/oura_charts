//---------------------------------------------------------------------------------------------------------------------
// datetime_helpers.h
//
// miscellaneous utility functions/wrappers used for working with date/time values and the chrono library
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <spanstream>
#include <chrono>
#include <string>
#include <string_view>

#if defined(__cpp_lib_chrono) && __cpp_lib_chrono < 201907L
   #include <date/date.h>
#endif

namespace oura_charts
{
   // import the namespaces and type aliases we'll use from chrono/date. These will be used
   // throughout the library and allow us to easily change clock type or duration in
   // one place.
   using namespace std::literals::chrono_literals;
   namespace chrono = std::chrono;
   using clock = chrono::system_clock;
   using chrono::sys_time;
   using chrono::sys_seconds;
   using chrono::sys_days;
   using chrono::days;
   using chrono::seconds;
   using chrono::local_time;
   using chrono::local_seconds;
   using chrono::local_days;
   using chrono::year_month_day;
   using chrono::hh_mm_ss;
   using chrono::time_point_cast;



// incomplete <chrono> in linux libstdc++ as of v12
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono < 201907L
   using date::from_stream;
   using date::parse;
#else
   using chrono::from_stream;
#endif


   /// <summary>
   ///   convenience function for convertiong local time to utc
   /// </summary>
   template <typename DurationT>
   [[nodiscard]] inline sys_time<DurationT> localToUtc(local_time<DurationT> ts, const chrono::time_zone* tz = chrono::current_zone())
   {
      return tz->to_sys(ts);
   }


   /// <summary>
   ///   convenience function for convertiong utc time to local
   /// </summary>
   template <typename DurationT>
   [[nodiscard]] inline local_time<DurationT> utcToLocal(sys_time<DurationT> ts, const chrono::time_zone* tz = chrono::current_zone())
   {
      return tz->to_local(ts);
   }


   /// <summary>
   ///   get the current time relative to the local time zone.
   /// </summary>
   [[nodiscard]] inline auto localNow()
   {
      return utcToLocal(clock::now());
   }


   /// <summary>
   ///   stip the time of day from a timepoint
   /// </summary>
   template <typename ClockT, typename DurationT>
   [[nodiscard]] inline chrono::time_point<ClockT, DurationT> stripTimeOfDay(chrono::time_point<ClockT, DurationT>  tp)
   {
      return time_point_cast<DurationT>(floor<days>(tp));
   }


   /// <summary>
   ///   Convert a time point into a calendar date and civil time.
   /// </summary>
   template<typename ClockT, typename DurationT>
   [[nodiscard]] inline std::pair<year_month_day, hh_mm_ss<DurationT>> getCivilTime(chrono::time_point<ClockT, DurationT> tp)
   {
      auto daypoint = floor<days>(tp);
      year_month_day day{ daypoint };
      hh_mm_ss tod{ tp - daypoint };

      return std::make_pair(day, tod);
   }


   /// <summary>
   ///   Parse an ISO date-time string and return it as a UTC timepoint. Values
   ///   returned from this function will always be UTC, even if the string
   ///   contained a timezone offeset. Expected value is the parsed date,
   ///   unexpected value is exception object containing error information.
   /// </summary>
   [[nodiscard]] inline expected<sys_seconds, oura_exception> parseIsoDateTime(std::string_view dt_str)
   {
      // first try parsing with time zone offset, if that fails try parsing as UTC.
      std::ispanstream dt_strm{ dt_str };
      sys_seconds ts{};

      dt_strm >> parse(constants::PARSE_FMT_STR_ISO_DATETIME_LOCAL, ts);
      if (dt_strm.fail())
      {
         dt_strm = std::ispanstream{ dt_str };
         dt_strm >> parse(constants::PARSE_FMT_STR_ISO_DATETIME_UTC, ts);
      }
      if (dt_strm.fail())
         return unexpected{ oura_exception{ fmt::format("The intput string '{}' could not be parsed as a valid date/time", dt_str), ErrorCategory::Parse } };
      else return ts;
   }


   /// <summary>
   ///   parse an ISO date string into a year_month_day
   /// </summary>
   [[nodiscard]] inline expected<chrono::year_month_day, oura_exception> parseIsoDate(std::string_view dt_str)
   {
      constexpr auto format_str{ constants::PARSE_FMT_STR_ISO_DATE_ONLY };

      std::ispanstream dt_strm{ dt_str, };
      sys_days days{};
      if ((dt_strm >> parse(format_str, days)))
         return year_month_day(days);
      else
         return unexpected{ oura_exception{ fmt::format("The intput string '{}' could not be parsed as a valid date", dt_str), ErrorCategory::Parse } };
   }


   /// <summary>
   ///   Format a date/time as ISO string that can be used with REST calls.
   /// </summary>
   /// <remarks>
   ///   This function should work for time_point's and month_day_year's, as well as any other
   ///   value that fmt::format() knows how to format as a date/time.
   /// </remarks>
   template<typename DateType>
   [[nodiscard]] inline std::string toIsoDateTime(DateType&& date_val)
   {
      return fmt::format("{:%FT%TZ}", chrono::floor<chrono::seconds>(std::forward<DateType>(date_val)) );
   }


   /// <summary>
   ///   Convert a year_month_day to a ISO date string
   /// </summary>
   [[nodiscard]] inline std::string toIsoDate(const chrono::year_month_day& date)
   {
      // fmt::format doesn't support the date types from C++ 20/23
      return std::format("{:%F}", date);
   }
}
