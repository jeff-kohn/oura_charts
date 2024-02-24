#pragma once

#include "oura_charts/constants.h"
#include <chrono>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <spanstream>

#if defined(__cpp_lib_chrono) && __cpp_lib_chrono < 201907L
   #include <date/date.h>
#endif

namespace oura_charts::detail
{
   namespace chrono = std::chrono;
   using clock = chrono::system_clock;
   using namespace std::literals::chrono_literals;

// incomplete <chrono> in linux libstdc++ as of v12
#if defined(_GLIBCXX_RELEASE) && _GLIBCXX_RELEASE > 12
   using date::from_stream;
#else
   using chrono::from_stream;
#endif


   /// <summary>
   ///   Parse an ISO date string and return it as a UTC timepoint. Values
   ///   returned from this function will always be UTC, even if the string
   ///   contained a timezone offeset. Expected value is the parsed date,
   ///   unexpected value is exception object containing error information.
   /// </summary>
   inline expected<clock::time_point, oura_exception> parseIsoDateTime(std::string_view dt_str)
   {

      auto format_str{ dt_str.ends_with(constants::UTC_TIMEZONE) ? constants::PARSE_FMT_STR_ISO_DATETIME_UTC
                                                                 : constants::PARSE_FMT_STR_ISO_DATETIME_LOCAL };

      std::ispanstream dt_strm{ dt_str, };
      clock::time_point tp{};
      if ( (dt_strm >> chrono::parse(format_str, tp)) )
         return tp;
      else
         return unexpected{ oura_exception{ fmt::format("The intput string {} could not be parsed as a valid date/time", dt_str), ErrorCategory::Parse }};
   }


   /// <summary>
   ///   Format a date/time as ISO string that can be used with REST calls.
   /// </summary>
   /// <remarks>
   ///   This function should work for time_point's and month_day_year's, as well as any other
   ///   value that fmt::format() knows how to format as a date/time.
   /// </remarks>
   template<typename DateType>
   inline std::string toIsoDateString(DateType&& date_val)
   {
      return fmt::format("{:%FT%TZ}", chrono::floor<chrono::seconds>(std::forward<DateType>(date_val)) );
   }

}
