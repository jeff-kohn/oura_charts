//---------------------------------------------------------------------------------------------------------------------
// datetime_helpers.h
//
// miscellaneous utility functions/wrappers used for working with date/time values and the chrono library
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

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
   using chrono::sys_days;
   using timestamp_utc = chrono::sys_seconds;
   using timestamp_local = chrono::local_seconds;


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
   inline timestamp_local localTimestamp(chrono::local_time<Duration> tp )
   {
      return chrono::floor<timestamp_local::duration>(tp);
   }


   inline auto toTimestamp(auto timepoint)
   {
      return chrono::floor<timestamp_utc::duration>(timepoint);
   }

   /// <summary>
   /// convenience function for convertiong timestamp_local to timestamp_utc
   /// </summary>
   inline timestamp_utc localToUtc(timestamp_local tp)
   {
      return chrono::floor<timestamp_utc::duration>(chrono::current_zone()->to_sys(tp));
   }

    
   /// <summary>
   ///   Parse an ISO date string and return it as a UTC timepoint. Values
   ///   returned from this function will always be UTC, even if the string
   ///   contained a timezone offeset. Expected value is the parsed date,
   ///   unexpected value is exception object containing error information.
   /// </summary>
   inline expected<timestamp_utc, oura_exception> parseIsoDateTime(std::string_view dt_str)
   {

      auto format_str{ dt_str.ends_with(constants::UTC_TIMEZONE) ? constants::PARSE_FMT_STR_ISO_DATETIME_UTC
                                                                 : constants::PARSE_FMT_STR_ISO_DATETIME_LOCAL };

      std::ispanstream dt_strm{ dt_str, };
      timestamp_utc ts{};
      if ( (dt_strm >> chrono::parse(format_str, ts)) )
         return ts;
      else
         return unexpected{ oura_exception{ fmt::format("The intput string {} could not be parsed as a valid date/time", dt_str), ErrorCategory::Parse }};
   }

   inline expected<timestamp_local, oura_exception> 


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

   template<typename DateType>
   inline std::string toIsoDate(DateType&& date_val)
   {
      return fmt::format("{:%F}", chrono::floor<chrono::seconds>(std::forward<DateType>(date_val)));
   }

}
