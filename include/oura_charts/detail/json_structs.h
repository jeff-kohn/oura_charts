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

namespace oura_charts::detail
{
   // REST API returns some string values as 'null', which the json parser
   // will choke on if you try to map it directly to a string.
   using nullible_string = std::optional<std::string>;

   template<typename Struct>
   struct RestDataSeries
   {
      std::vector<Struct> data;
      nullible_string next_token;
   };

   struct hr_data
   {
      int bpm;
      std::string source;
      std::string timestamp;
   };
   using hr_data_series = RestDataSeries<hr_data>;

} // namespace oura_charts::detail
