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
#include <glaze/glaze.hpp>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace oura_charts::detail
{
   // REST API may return some values as 'null', which the json parser
   // will choke on if you try to map it directly to a string or built-in type.
   template <typename T>
   using nullable = std::optional<T>;
   using nullable_string = std::optional<std::string>;
   using nullable_double = std::optional<double>;
   using nullable_int    = std::optional<int32_t>;
   using nullable_uint   = std::optional<uint32_t>;


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
      nullable_string next_token{};
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

      struct readiness_data
      {
         std::map<ReadinessContributors, int> contributors{};
         int score{};
         nullable_double temperature_deviation{};
         nullable_double temperature_trend_deviation{};
      };

      struct interval_data
      {
         chrono::seconds interval{};
         std::vector<nullable_double> items;
         local_seconds timeststamp{};
      };

      std::string id{};
      SleepType type{};
      int period{};

      year_month_day day{};
      local_seconds bedtime_start{};
      local_seconds bedtime_end{};

      nullable_double average_breath{};

      nullable<interval_data> heart_rate{};
      nullable_double average_heart_rate{};
      nullable_uint lowest_heart_rate{};

      nullable<interval_data> hrv{};
      nullable_double average_hrv{};
      

      int efficiency{};
      chrono::seconds latency{};
      nullable_uint restless_periods{};
      std::string movement_30_sec{};
      std::string sleep_phase_5_min{};

      chrono::seconds time_in_bed{};
      chrono::seconds awake_time{};
      chrono::seconds total_sleep_duration{};
      chrono::seconds light_sleep_duration{};
      chrono::seconds deep_sleep_duration{};
      chrono::seconds rem_sleep_duration{};

      readiness_data readiness{};
      nullable_uint readiness_score_delta{};
      nullable_uint sleep_score_delta{};
   };

   struct daily_sleep_data
   {
      enum class SleepScoreContributors
      {
         deep_sleep,
         efficiency,
         latency,
         rem_sleep,
         restfulness,
         timing,
         total_sleep
      };

      std::string id{};
      year_month_day day{};

      std::map<SleepScoreContributors, int> contributors{};
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
   struct from_json<oc::chrono::year_month_day>
   {
      template <auto Opts>
      static void op(oc::chrono::year_month_day& value, is_context auto&& ctx, auto&&... args)
      {
         std::string date_str{};
         read<json>::op<Opts>(date_str, ctx, args...);
         auto ymd_res = oc::parseIsoDate(date_str);
         if (ymd_res)
            value = ymd_res.value();
         else
            ctx.error = glz::error_code::parse_number_failure;
      }
   };

   template <>
   struct from_json<oc::local_seconds>
   {
      template <auto Opts>
      static void op(oc::local_seconds& value, is_context auto&& ctx, auto&&... args)
      {
         std::string date_str{};
         read<json>::op<Opts>(date_str, ctx, args...);
         auto tp_res = oc::parseIsoDateTime(date_str);
         if (tp_res)
            value = oc::utcToLocal(tp_res.value());
         else
            ctx.error = glz::error_code::parse_number_failure;
      }
   };

   template <>
   struct from_json<oc::chrono::seconds>
   {
      template <auto Opts>
      static void op(oc::chrono::seconds& value, is_context auto&& ctx, auto&&... args)
      {
         int32_t sec_count{};
         read<json>::op<Opts>(sec_count, ctx, args...);
         if (glz::error_code::none == ctx.error)
            value = oc::chrono::seconds{ sec_count };
      }
   };

} // namespace oura_charts::detail

template <>
struct glz::meta<oura_charts::detail::sleep_data::SleepType>
{
   using enum oura_charts::detail::sleep_data::SleepType;
   static constexpr auto value = enumerate(
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
   static constexpr auto value = enumerate(
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
