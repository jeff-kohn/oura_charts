//---------------------------------------------------------------------------------------------------------------------
// json_structs.h
//
// Declaration for binding structs used for parsing JSON text
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/chrono_helpers.h"
#include "oura_charts/detail/nullable_types.h"
#include <glaze/glaze.hpp>
#include <map>
#include <string>
#include <vector>

namespace oura_charts::detail
{



   /// <summary>
   ///   JSON struct for personal info profile from Oura
   /// </summary>
   struct profile_data
   {
      std::string id{};
      std::string email{};
      int age{};
      double weight{};
      double height{};
      std::string biological_sex{};
   };


   /// <summary>
   ///   This is the "outer" struct used with REST calls to the Oura API.
   /// </summary>
   /// <remarks>
   ///   All of the Oura API's that return a series of data use the same format
   ///   with an array to receive the requested data structs, and a token indicating
   ///   whether there is more data available to fullfill the request (in case
   ///   of a very large number of data structs, paging may be used).
   /// </remarks>
   template<typename T>
   struct RestDataCollection
   {
      using value_type = T;
      std::vector<value_type> data{};
      NullableString next_token{};
   };


   /// <summary>
   ///   struct that contains information about a single heart rate measurement.
   /// </summary>
   struct hr_data
   {
      int bpm{};
      std::string source{};
      local_seconds timestamp{};
   };


   /// <summary>
   ///   aggregate struct that contains information about a single sleep session.
   /// </summary>
   struct sleep_data
   {
      enum class SleepType
      {
         any = 0,
         rest,
         late_nap,
         sleep,
         long_sleep,
      };

      enum class ReadinessContributors
      {
         activity_balance,
         body_temperature,
         hrv_balance,
         previous_day_activity,
         previous_night,
         recovery_index,
         resting_heart_rate,
         sleep_balance
      };

      using ReadinessContributorsMap = std::map<ReadinessContributors, int>;
      struct ReadinessData
      {
         ReadinessContributorsMap contributors{};
         int score{};
         NullableDouble temperature_deviation{};
         NullableDouble temperature_trend_deviation{};
      };

      struct IntervalData
      {
         chrono::seconds interval{};
         std::vector<NullableDouble> items;
         local_seconds timeststamp{};
      };

      std::string id{};
      SleepType type{};
      int period{};

      year_month_day day{};
      local_seconds bedtime_start{};
      local_seconds bedtime_end{};

      NullableDouble average_breath{};

      Nullable<IntervalData> heart_rate{};
      NullableDouble average_heart_rate{};
      NullableUInt lowest_heart_rate{};

      Nullable<IntervalData> hrv{};
      NullableDouble average_hrv{};
      

      int efficiency{};
      chrono::seconds latency{};
      NullableUInt restless_periods{};
      std::string movement_30_sec{};
      std::string sleep_phase_5_min{};

      chrono::seconds time_in_bed{};
      chrono::seconds awake_time{};
      chrono::seconds total_sleep_duration{};
      chrono::seconds light_sleep_duration{};
      chrono::seconds deep_sleep_duration{};
      chrono::seconds rem_sleep_duration{};



      ReadinessData readiness{};
      NullableUInt readiness_score_delta{};
      NullableUInt sleep_score_delta{};
   };


   struct daily_sleep_data
   {
      struct SleepScoreContributors
      {
         int deep_sleep{};
         int efficiency{};
         int latency{};
         int rem_sleep{};
         int restfulness{};
         int timing{};
         int total_sleep{};
      };

      std::string id{};
      year_month_day day{};
      int score{};
      SleepScoreContributors contributors{};
      local_seconds timestamp{};
   };

   /// <summary>
   ///   result type used for parsing JSON text into struct data.
   /// </summary>
   /// <typeparam name="T"></typeparam>
   template<typename T>
   using ParseResult = expected<T, oura_exception>;


   /// <summary>
   ///   wrapper for glz::read<> that returns an expected<> instead of an error code (eliminating the
   ///   need to pass the struct as a parameter or translate any parse_error's returned.
   /// </summary>
   template <glz::opts Opts, typename ValueT, StringViewCompatible StringT>
   [[nodiscard]] inline ParseResult<ValueT> readJson(StringT&& buffer) noexcept
   {
      ValueT value{};
      auto&& pe = glz::read<Opts>(value, buffer);
      if (pe)
         return unexpected(oura_exception{ static_cast<int64_t>(pe.ec), glz::format_error(pe, buffer), ErrorCategory::Parse });
      else
         return value;
   }


   /// <summary>
   ///   wrapper for glz::read<> that sets the "error_on_unknown_keys" compile-time option and returns
   ///   an expected<> for the value (or error).
   /// </summary>
   /// <remarks>
   ///   if you want to explicitly set glz compile-time options, use the other overload.
   /// <remarks>
   template <typename ValueT, StringViewCompatible StringT>
   [[nodiscard]] inline ParseResult<ValueT> readJson(StringT&& buffer) noexcept
   {
      return readJson < glz::opts{ .error_on_unknown_keys = false }, ValueT > (buffer);
   }

} // namespace oura_charts::detail


/// <summary>
///   Custom json serializers used with glaze library for chrono types we use.
/// </summary>
namespace glz::detail
{
   namespace oc = oura_charts;

   template <>
   struct from<JSON, oc::chrono::year_month_day>
   {
      template <auto Opts>
      static void op(oc::chrono::year_month_day& value, is_context auto&& ctx, auto&&... args)
      {
         std::string date_str{};
         read<JSON>::op<Opts>(date_str, ctx, args...);
         auto ymd_res = oc::parseIsoDate(date_str);
         if (ymd_res)
            value = ymd_res.value();
         else
            ctx.error = glz::error_code::parse_number_failure;
      }
   };

   template <>
   struct from<JSON, oc::local_seconds>
   {
      template <auto Opts>
      static void op(oc::local_seconds& value, is_context auto&& ctx, auto&&... args)
      {
         std::string date_str{};
         read<JSON>::op<Opts>(date_str, ctx, args...);
         auto tp_res = oc::parseIsoDateTime(date_str);
         if (tp_res)
            value = oc::utcToLocal(tp_res.value());
         else
            ctx.error = glz::error_code::parse_number_failure;
      }
   };

   template <>
   struct from<JSON, oc::chrono::seconds>
   {
      template <auto Opts>
      static void op(oc::chrono::seconds& value, is_context auto&& ctx, auto&&... args)
      {
         int32_t sec_count{};
         read<JSON>::op<Opts>(sec_count, ctx, args...);
         if (glz::error_code::none == ctx.error)
            value = oc::chrono::seconds{ sec_count };
      }
   };

} // namespace oura_charts::detail

template <>
struct glz::meta<oura_charts::detail::sleep_data::SleepType>
{
   using enum oura_charts::detail::sleep_data::SleepType;
   static inline constexpr auto value = enumerate(
         rest,
         late_nap,
         sleep,
         long_sleep
   );
};

template <>
struct glz::meta<oura_charts::detail::sleep_data::ReadinessContributors>
{
   using enum oura_charts::detail::sleep_data::ReadinessContributors;
   static inline constexpr auto value = enumerate(
         activity_balance,
         body_temperature,
         hrv_balance,
         previous_day_activity,
         previous_night,
         recovery_index,
         resting_heart_rate,
         sleep_balance
   );
};
