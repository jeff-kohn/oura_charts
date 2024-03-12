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
   using std::string;
   using std::vector;


   // REST API may return some values as 'null', which the json parser
   // will choke on if you try to map it directly to a string or built-in type.
   using nullable_string = std::optional<string>;
   using nullable_number = std::optional<double>;


   template<typename Struct>
   struct RestDataCollection
   {
      std::vector<Struct> data;
      nullable_string next_token;
   };

   struct hr_data
   {
      int bpm;
      string source;
      string timestamp;
   };
   using hr_data_series = RestDataCollection<hr_data>;



   struct sleep_data
   {
      chrono::year_month_day day;
      timestamp_local bedtime_start{};
      timestamp_local bedtime_end{};

      nullable_number average_breath{};
      nullable_number average_heart_rate{};
      nullable_number average_hrv{};

      nullable_number lowest_heart_rate{};

      chrono::seconds latency{};
      chrono::seconds awake_time{};
      chrono::seconds deep_sleep_duration{};
      chrono::seconds light_sleep_duration{};
      chrono::seconds rem_sleep_duration{};

      nullable_number restless_periods{};

   };
   using sleep_data_series = RestDataCollection<sleep_data>;

} // namespace oura_charts::detail


namespace oc = oura_charts;
namespace ocd = oura_charts::detail;


namespace glz::detail
{
   template <>
   struct from_json<oc::timestamp_local>
   {
      template <auto Opts>
      static void op(oc::timestamp_local& value, auto&&... args)
      {
         std::string date_str{};
         read<json>::op<Opts>(date_str, args...);
         value  = 
      }
   };
}



//// metadata specialization to allow for customizing the parsing behavior
//// for our class.
//template <>
//struct glz::meta<ocd::sleep_data>
//{
//   using T = ocd::sleep_data;
//   static constexpr auto partial_read = true;
//   static constexpr auto value = object("day", custom<&T::read_x, &T::write_x>,
//                                          "y", custom<&T::read_y, &T::y>, 
//                                          "z", custom<&T::z, &T::write_z>);
//   };
//};
