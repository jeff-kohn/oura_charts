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
   using chrono::year_month_day;
   using chrono::local_time;
   using chrono::local_seconds;

   

// incomplete <chrono> in linux libstdc++ as of v12
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono < 201907L
   using date::from_stream;
#else
   using chrono::from_stream;
#endif

   /// <summary>
   ///   get the current time relative to the local time zone.
   /// </summary>
   inline [[nodiscard]] auto localNow()
   {
      auto utc_now = clock::now();
      return chrono::current_zone()->to_local(utc_now);
   }


   /// <summary>
   ///   convenience function for convertiong local time to utc
   /// </summary>
   template <typename DurationT>
   inline [[nodiscard]] chrono::sys_time<DurationT> localToUtc(chrono::local_time<DurationT> ts)
   {
      return chrono::current_zone()->to_sys(ts);
   }


   /// <summary>
   ///   convenience function for convertiong utc time to local
   /// </summary>
   template <typename DurationT>
   inline [[nodiscard]] local_time<DurationT> utcToLocal(sys_time<DurationT> ts, const chrono::time_zone* tz = chrono::current_zone())
   {
      return tz->to_local(ts);
   }

    
   /// <summary>
   ///   Parse an ISO date-time string and return it as a UTC timepoint. Values
   ///   returned from this function will always be UTC, even if the string
   ///   contained a timezone offeset. Expected value is the parsed date,
   ///   unexpected value is exception object containing error information.
   /// </summary>
   inline [[nodiscard]] expected<sys_seconds, oura_exception> parseIsoDateTime(std::string_view dt_str)
   {

      auto format_str{ dt_str.ends_with(constants::UTC_TIMEZONE) ? constants::PARSE_FMT_STR_ISO_DATETIME_UTC
                                                                 : constants::PARSE_FMT_STR_ISO_DATETIME_LOCAL };
      std::ispanstream dt_strm{ dt_str, };
      sys_seconds ts{};
      if ( (dt_strm >> chrono::parse(format_str, ts)) )
         return ts;
      else
         return unexpected{ oura_exception{ fmt::format("The intput string '{}' could not be parsed as a valid date/time", dt_str), ErrorCategory::Parse }};
   }


   /// <summary>
   ///   parse an ISO date string into a year_month_day
   /// </summary>
   inline [[nodiscard]] expected<chrono::year_month_day, oura_exception> parseIsoDate(std::string_view dt_str)
   {
      constexpr auto format_str{ constants::PARSE_FMT_STR_ISO_DATE_ONLY };
                                                                
      std::ispanstream dt_strm{ dt_str, };
      year_month_day ymd{};
      if ((dt_strm >> chrono::parse(format_str, ymd)))
         return ymd;
      else
         return unexpected{ oura_exception{ fmt::format("The intput string {} could not be parsed as a valid date", dt_str), ErrorCategory::Parse } };
   }


   /// <summary>
   ///   Format a date/time as ISO string that can be used with REST calls.
   /// </summary>
   /// <remarks>
   ///   This function should work for time_point's and month_day_year's, as well as any other
   ///   value that fmt::format() knows how to format as a date/time.
   /// </remarks>
   template<typename DateType>
   inline [[nodiscard]] std::string toIsoDateTime(DateType&& date_val)
   {
      return fmt::format("{:%FT%TZ}", chrono::floor<chrono::seconds>(std::forward<DateType>(date_val)) );
   }


   /// <summary>
   ///   Convert a year_month_day to a ISO date string
   /// </summary>
   inline [[nodiscard]] std::string toIsoDate(const chrono::year_month_day& date)
   {
      return std::format("{:%F}", date);
   }
}
