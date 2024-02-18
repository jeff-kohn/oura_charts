#pragma once

#include "oura_charts/constants.h"
#include <chrono>
#include <string>
#include <fmt/format.h>

namespace oura_charts::datetime
{
   namespace chrono = std::chrono;
   using clock = chrono::utc_clock;
   using ms = chrono::milliseconds;
   using namespace std::literals::chrono_literals;


   /// <summary>
   ///   Parse an ISO date string and return it as a UTC timepoint. Values
   ///   returned from this function will always be UTC, even if the string
   ///   contained a timezone offeset. Throws oura_exception if the string
   ///   couldn't be parsed.
   /// </summary>
   inline std::expected<clock::time_point, oura_exception> parseIsoDateTime(std::string_view dt_str)
   {
      std::istringstream ss{ dt_str.ends_with(constants::UTC_TIMEZONE) ? constants::ISO_DATETIME_UTC_FORMAT
                                                                       : constants::ISO_DATETIME_LOCAL_FORMAT };

      clock::time_point tp;
      if (chrono::from_stream(ss, "%FT%T", tp))
         return tp;
      else
         return std::unexpected{ oura_exception(fmt::format("The intput string {} could not be parsed as a valid date/time", dt_str)) };
   }

}
