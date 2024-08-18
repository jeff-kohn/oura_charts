//---------------------------------------------------------------------------------------------------------------------
// chrono_helpers.h
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
#include <functional>
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

   template <typename DurationT>
   using sys_time = chrono::sys_time<DurationT>;
   using sys_seconds = chrono::sys_seconds;
   using sys_days = chrono::sys_days;

   template <typename DurationT>
   using local_time = chrono::local_time<DurationT>;
   using local_seconds = chrono::local_seconds;
   using local_days = chrono::local_days;
          
   using seconds = std::chrono::seconds;
   using minutes = chrono::minutes;
   using hours = chrono::hours;
   using days = chrono::days;
   using weeks = chrono::weeks;
   using months = chrono::months;
   using years = chrono::years;
          
   using year_month_day = chrono::year_month_day;
   using year_month = chrono::year_month;
   using month = chrono::month;
   using year = chrono::year;
   using weekday = chrono::weekday;

   template <typename DurationT>
   using hh_mm_ss = chrono::hh_mm_ss<DurationT>;


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
   [[nodiscard]] inline constexpr auto getWeekdays(weekday sortFirst = {chrono::Sunday})
   {
         constexpr int weekday_count = 7;
         std::array<weekday, weekday_count> weekdays{};
         std::iota(rg::begin(weekdays), rg::end(weekdays), sortFirst);
         return weekdays;
   }

   /// <summary>
   ///   small helper to generate a sorted array containing the names of the days of the week
   /// </summary>
   [[nodiscard]] inline constexpr auto getWeekdayNames(weekday sortFirst = { chrono::Sunday })
   {
      auto days = getWeekdays(sortFirst);

      return days | vw::transform([] (weekday day)
                                 {
                                    return fmt::format("{}", day);
                                 })
                  | vw::as_rvalue
                  | rg::to<std::vector>();
   }

   /// <summary>
   ///   small helper to generate a sorted array containing the days of the week
   /// </summary>
   [[nodiscard]] inline constexpr auto getMonths()
   {
      using namespace chrono;
      constexpr std::array months{ January, February, March, April, May, June, July, August, September, October, November, December};
      return months;
   }



   /// <summary>
   ///   convert a year_month_day into a year_month. trivial convesion, but doing it
   ///   'inline' will often be ugly/tedious.
   /// </summary>
   [[nodiscard]] inline constexpr year_month stripDay(year_month_day val)
   {
      return year_month{ val.year(), val.month() };
   }


   /// <summary>
   ///   convert a year_month_day into a year_month. trivial convesion, but doing it
   ///   'inline' will often be ugly/tedious.
   /// </summary>
   [[nodiscard]] inline constexpr year stripDayAndMonth(year_month_day val)
   {
      return year{ val.year() };
   }



   //
   // These lambda's can be used as projections for converting member functions that return year_month_day (or compatible)
   // to various calandar types.
   //
   template <auto memFunc>
   inline constexpr auto selectAsYearMonthDay = [] (const auto& obj) -> year_month_day requires InvocableFor<decltype(memFunc), decltype(obj)>
      {
         return year_month_day{ std::invoke(memFunc, obj) };
      };

   template <auto memFunc>
   inline constexpr auto selectAsYear = [](const auto& obj) -> year requires InvocableFor<decltype(memFunc), decltype(obj)>
      {
         return year_month_day{ std::invoke(memFunc, obj) }.year();
      };

   template <auto memFunc>
   inline constexpr auto selectAsYearMonth = [](const auto& obj) -> year_month requires InvocableFor<decltype(memFunc), decltype(obj)>
      {
         return year_month{ std::invoke(memFunc, obj) };
      };

   template <auto memFunc>
   inline constexpr auto selectAsMonth = [] (const auto& obj) -> month requires InvocableFor<decltype(memFunc), decltype(obj)>
      {
         return year_month_day{ std::invoke(memFunc, obj) }.month();
      };

   template <auto memFunc>
   inline constexpr auto selectAsWeekday = [] (const auto& obj) -> weekday requires InvocableFor<decltype(memFunc), decltype(obj)>
      {
         return sys_days{ std::invoke(memFunc, obj) };
      };



   /// <summary>
   ///   AvgCalc<> specialization for chrono duration types
   ///  
   ///   if you get compile error trying to pass this object type to an algorithm, it's because
   ///   this functor is move-only, you need to use std::ref() with algorithms that accept their
   ///   functor by-value.
   /// </summary>
   template<detail::ChronoDuration DurationT>
   class AvgCalc<DurationT, double>
   {
   public:
      using Rep = DurationT::rep;
      using InputType = DurationT;
      using NullableInputType = std::optional<InputType>;
      using ResultType = std::optional<InputType>;
      
      void operator()(const InputType& val) noexcept
      {
         m_sum(val.count());
         ++m_count;
      }

      void operator()(const NullableInputType& val) noexcept
      {
         if (val.has_value())
            (*this)(val.value());
      };


      /// <summary>
      ///   return the result that was calculated. may be a null/empty optional if operator() was
      ///   never called with a non-empty value.
      /// </summary>
      ResultType result() const noexcept
      {
         // Hitting this means you either averaged an empty/null-only range, or you forgot to
         // use std::ref() when passing the functor to a std:: algorithm.
         assert(m_count != 0);

         if (m_sum.hasResult())
         {
            auto sum = m_sum.result().value();
            InputType result_val{ static_cast<Rep>(sum / m_count) };
            return ResultType{ result_val };
         }
         else
         {
            return ResultType{};
         }
      }

      size_t count() const noexcept
      {
         return m_count;
      }

      // returns true if a non-null/empty value has been calculated. If this returns
      // false result() will return an empty std::optional<>
      bool hasResult() const noexcept
      {
         return m_sum.hasResult();
      }

   private:
      SumCalc<Rep, double> m_sum{};
      size_t m_count{};
   };


}
