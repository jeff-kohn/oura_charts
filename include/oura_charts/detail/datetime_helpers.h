#pragma once

#include "oura_charts/constants.h"
#include <chrono>
#include <fmt/format.h>
#include <fmt/chrono.h>

#if defined(__cpp_lib_chrono) && __cpp_lib_chrono < 201907L
   #include <date/date.h>
#endif

namespace oura_charts::datetime
{
   namespace chrono = std::chrono;
   using ms = chrono::milliseconds;
   using namespace std::literals::chrono_literals;

// libstdc++ currrently has incomplete c++ 20 impl of chrono
// this version number may need to be increased depending on
// when chrono is complete.
#if _GLIBCXX_RELEASE > 12
   using clock = chrono::utc_clock;
   using chrono::from_stream;
#else
   using clock = chrono::system_clock;
   using date::from_stream;
#endif


   /// <summary>
   ///   Parse an ISO date string and return it as a UTC timepoint. Values
   ///   returned from this function will always be UTC, even if the string
   ///   contained a timezone offeset. Throws oura_exception if the string
   ///   couldn't be parsed.
   /// </summary>
   inline expected<clock::time_point, oura_exception> parseIsoDateTime(std::string_view dt_str)
   {
      std::istringstream ss{ dt_str.ends_with(constants::UTC_TIMEZONE) ? constants::ISO_DATETIME_UTC_FORMAT
                                                                       : constants::ISO_DATETIME_LOCAL_FORMAT };

      clock::time_point tp;
      if (from_stream(ss, "%FT%T", tp))
         return tp;
      else
         return unexpected{ oura_exception(fmt::format("The intput string {} could not be parsed as a valid date/time", dt_str)) };
   }

}
