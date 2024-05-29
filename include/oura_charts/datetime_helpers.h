//---------------------------------------------------------------------------------------------------------------------
// datetime_helpers.h
//
// miscellaneous utility functions/wrappers used for working with date/time values and the chrono library
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/constants.h"
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
   using utc_date = chrono::sys_days;
   using utc_timestamp = chrono::sys_seconds;
   using local_date = chrono::local_days;
   using local_timestamp = chrono::local_seconds;


// incomplete <chrono> in linux libstdc++ as of v12
#if defined(__cpp_lib_chrono) && __cpp_lib_chrono < 201907L
   using date::from_stream;
#else
   using chrono::from_stream;
#endif


   inline auto localNow()
   {
      auto utc_now = clock::now();
      return chrono::current_zone()->to_local(utc_now);
   }

   template <typename Duration>
   inline local_timestamp localTimestamp(chrono::local_time<Duration> tp )
   {
      return chrono::floor<local_timestamp::duration>(tp);
   }


   /*inline auto toTimestamp(auto timepoint)
   {
      return chrono::floor<utc_timestamp::duration>(timepoint);
   }*/


   /// <summary>
   ///   convenience function for convertiong local_timestamp to utc_timestamp
   /// </summary>
   inline utc_timestamp localToUtc(local_timestamp ts)
   {
      return chrono::floor<utc_timestamp::duration>(chrono::current_zone()->to_sys(ts));
   }

   /// <summary>
   ///   convenience function for convertiong local_timestamp to utc_timestamp
   /// </summary>
   inline local_timestamp utcToLocal(utc_timestamp ts, const chrono::time_zone* tz = chrono::current_zone())
   {
      return chrono::floor<local_timestamp::duration>(tz->to_local(ts));
   }

    
   /// <summary>
   ///   Parse an ISO date-time string and return it as a UTC timepoint. Values
   ///   returned from this function will always be UTC, even if the string
   ///   contained a timezone offeset. Expected value is the parsed date,
   ///   unexpected value is exception object containing error information.
   /// </summary>
   inline expected<utc_timestamp, oura_exception> parseIsoDateTime(std::string_view dt_str)
   {

      auto format_str{ dt_str.ends_with(constants::UTC_TIMEZONE) ? constants::PARSE_FMT_STR_ISO_DATETIME_UTC
                                                                 : constants::PARSE_FMT_STR_ISO_DATETIME_LOCAL };

      std::ispanstream dt_strm{ dt_str, };
      utc_timestamp ts{};
      if ( (dt_strm >> chrono::parse(format_str, ts)) )
         return ts;
      else
         return unexpected{ oura_exception{ fmt::format("The intput string {} could not be parsed as a valid date/time", dt_str), ErrorCategory::Parse }};
   }


   /// <summary>
   /// 
   /// </summary>
   inline expected<chrono::year_month_day, oura_exception> parseIsoDate(std::string_view dt_str)
   {

      auto format_str{ constants::PARSE_FMT_STR_ISO_DATE_ONLY };
                                                                
      std::ispanstream dt_strm{ dt_str, };
      chrono::year_month_day ymd{};
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
   inline std::string toIsoDateTime(DateType&& date_val)
   {
      return fmt::format("{:%FT%TZ}", chrono::floor<chrono::seconds>(std::forward<DateType>(date_val)) );
   }


   /// <summary>
   /// 
   /// </summary>
   template<typename DateType>
   inline std::string toIsoDate(DateType&& date_val)
   {
      return fmt::format("{:%F}", chrono::floor<chrono::seconds>(std::forward<DateType>(date_val)));
   }

}
