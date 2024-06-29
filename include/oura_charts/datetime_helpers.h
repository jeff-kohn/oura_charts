//---------------------------------------------------------------------------------------------------------------------
// datetime_helpers.h
//
// miscellaneous utility functions/wrappers used for working with date/time values and the chrono library
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/functors.h"
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <cassert>
#include <chrono>
#include <ranges>
#include <spanstream>
#include <string>
#include <string_view>


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
  
   using chrono::local_time;
   using chrono::local_seconds;
   using chrono::local_days;

   using chrono::seconds;
   using chrono::minutes;
   using chrono::hours;
   using chrono::days;
   using chrono::weeks;
   using chrono::months;
   using chrono::years;

   using chrono::year_month_day;
   using chrono::weekday;
   using chrono::hh_mm_ss;
 
   using chrono::time_point_cast;


   namespace detail
   {
      using chrono::duration;

      template <class T>
      struct is_duration : std::false_type {};

      template <class RepT, class PeriodT>
      struct is_duration<duration<RepT, PeriodT> > : std::true_type  {};

      template <class RepT, class PeriodT>
      struct is_duration<const duration<RepT, PeriodT> > : std::true_type  {};

      template <class RepT, class PeriodT>
      struct is_duration<volatile duration<RepT, PeriodT> > : std::true_type  {};

      template <class RepT, class PeriodT>
      struct is_duration<const volatile duration<RepT, PeriodT> > : std::true_type  {};

      /// <summary>
      ///   Concept requiring a type to be a chrono::duration
      /// </summary>
      template <typename T>
      concept ChronoDuration = is_duration<T>::value;

   } // namespace detail


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
   ///   For a given time_point, return a time_point that represents only the date portion of the original time_point
   ///   (in other words, midnight).
   /// </summary>
   /// <remarks>
   ///   return value is still same type as passed-in value, just without the TOD. If you need a year_month_day call
   ///   getCalendarDate()
   template <typename ClockT, typename DurationT>
   [[nodiscard]] inline constexpr chrono::time_point<ClockT, DurationT> stripTimeOfDay(chrono::time_point<ClockT, DurationT>  tp)
   {
      return floor<days>(tp);
   }

   /// <summary>
   ///   For a given time_point, return a time_point that represents only the date portion of the original time_point
   ///   (in other words, midnight).
   /// </summary>
   /// <remarks>
   ///   return value is still same type as passed-in value, just without the TOD. If you need a year_month_day call
   ///   getCalendarDate()
   template <typename ClockT, typename DurationT>
   [[nodiscard]] inline constexpr year_month_day getCalendarDate(chrono::time_point<ClockT, DurationT>  tp)
   {
      return year_month_day{ floor<days>(tp) };
   }


   /// <summary>
   ///   For a given time_point, return a duration that represents only the time of day (no date).
   /// </summary>
   template <typename ClockT, typename DurationT>
   [[nodiscard]] inline constexpr ClockT::duration getTimeOfDayDuration(chrono::time_point<ClockT, DurationT>  tp)
   {
      auto daypoint = floor<days>(tp);
      return tp - daypoint;
   }



   /// <summary>
   ///   Convert a time point into a calendar date and civil time.
   /// </summary>
   template<typename ClockT, typename DurationT>
   [[nodiscard]] inline constexpr std::pair<year_month_day, hh_mm_ss<DurationT>> getCivilTime(chrono::time_point<ClockT, DurationT> tp)
   {
      auto daypoint = floor<days>(tp);
      hh_mm_ss tod{ tp - daypoint };
      year_month_day day{ daypoint };

      return std::make_pair(day, tod);
   }


   /// <summary>
   ///   convert a year_month_day and (option) hh_mm_ss to a UTC time_point
   /// </summary>
   /// <remarks>
   ///   if no time of day is supplied, you'll get midnight UTC.
   /// </remarks>
   template<typename ClockT = clock, typename DurationT = clock::duration>
   [[nodiscard]] inline constexpr clock::time_point civilTimeToClockTime(year_month_day ymd, hh_mm_ss<DurationT> tod = {})
   {
      chrono::time_point<ClockT, days>  day_point{ ymd };
      return day_point + tod.to_duration();
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


   /// <summary>
   ///   operator '<' comparison for weekday based on c_encoding() so we can 
   ///   store them in ordered containers like maps.
   /// </summary>
   struct weekday_compare_less
   {
      bool operator()(const weekday& lhs, const weekday& rhs) const noexcept
      {
         return lhs.c_encoding() < rhs.c_encoding();
      }
   };

   
   /// <summary>
   ///   small helper to generate a sorted array containing the days of the week
   /// </summary>
   [[nodiscard]] inline constexpr auto getWeekdays(weekday sortFirst = {chrono::Monday})
   {
         constexpr int weekday_count = 7;
         std::array<weekday, weekday_count> weekdays{};
         std::iota(rg::begin(weekdays), rg::end(weekdays), sortFirst);
         return weekdays;
   }


   /// Use parttial specialization with concept to make AvgCalc work with duration types
   template<detail::ChronoDuration DurationT>
   class AvgCalc<DurationT>
   {
   public:
      using Duration = DurationT;
      using Rep = Duration::rep;
      
      void operator()(const Duration& val) noexcept
      {
         m_sum += val.count();
         ++m_count;
      }

      Duration result() const noexcept
      {
         // Hitting this means you either averaged an empty range, or you forgot to
         // use std::ref() when passing the functor to a std:: algorithm.
         assert(m_sum != 0);

         return Duration{ static_cast<Rep>( m_sum / m_count) };
      }

      size_t count()const noexcept
      {
         return m_count;
      }

   private:
      double m_sum{};
      size_t m_count{};
   };


}
