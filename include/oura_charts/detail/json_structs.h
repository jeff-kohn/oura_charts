//---------------------------------------------------------------------------------------------------------------------
// json_structs.h
//
// Declaration for binding structs used for parsing JSON text
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <optional>
#include <oura_charts/datetime_helpers.h>
#include <glaze/glaze.hpp>

namespace oura_charts::detail
{
   // REST API may return some values as 'null', which the json parser
   // will choke on if you try to map it directly to a string or built-in type.
   using nullable_string = std::optional<std::string>;
   using nullable_double = std::optional<double>;

   /// <summary>
   ///   JSON struct for personal info profile from Oura
   /// </summary>
   struct user_data
   {
      std::string id;
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
   template<typename Struct>
   struct RestDataCollection
   {
      std::vector<Struct> data;
      nullable_string next_token;
   };

   /// <summary>
   ///   struct that contains information about a single heart rate measurement.
   /// </summary>
   struct hr_data
   {
      int bpm;
      std::string source;
      std::string timestamp;
   };
   using hr_data_series = RestDataCollection<hr_data>;      


   /// <summary>
   ///   struct that contains information about a single sleep session.
   /// </summary>
   struct sleep_data
   {
      std::string id;
      chrono::year_month_day day;
      timestamp_local bedtime_start{};
      timestamp_local bedtime_end{};

      nullable_double average_breath{};
      nullable_double average_heart_rate{};
      nullable_double average_hrv{};

      nullable_double lowest_heart_rate{};

      chrono::seconds latency{};
      chrono::seconds awake_time{};
      chrono::seconds deep_sleep_duration{};
      chrono::seconds light_sleep_duration{};
      chrono::seconds rem_sleep_duration{};

      nullable_double restless_periods{};

   };
   using sleep_data_series = RestDataCollection<sleep_data>;

} // namespace oura_charts::detail


namespace oc = oura_charts;
namespace ocd = oura_charts::detail;


/// <summary>
///   Custom json serializers used with glaze library for chrono types we use.
/// </summary>
namespace glz::detail
{
   template <>
   struct from_json<oc::chrono::year_month_day>
   {
      template <auto Opts>
      static void op(oc::chrono::year_month_day& value, is_context auto&& ctx, auto&&... args)
      {
         std::string date_str{};
         read<json>::op<Opts>(date_str, ctx, args...);
         auto exp_ymd = oc::parseIsoDate(date_str);
         if (exp_ymd)
            value = exp_ymd.value();
         else
            ctx.error = glz::error_code::parse_number_failure;
      }
   };

   template <>
   struct from_json<oc::timestamp_local>
   {
      template <auto Opts>
      static void op(oc::timestamp_local& value, is_context auto&& ctx, auto&&... args)
      {
         std::string date_str{};
         read<json>::op<Opts>(date_str, ctx, args...);
         auto exp_tsl = oc::parseIsoDateTime(date_str);
         if (exp_tsl)
            value = oc::utcToLocal(exp_tsl.value());
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
}
