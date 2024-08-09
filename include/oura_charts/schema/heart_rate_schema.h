#pragma once

namespace oura_charts::schema
{

   constexpr auto HeartRate_Schema =
      R"({
      "class_name": "HeartRate",
      "supported_groupings": [
         {
            "field": "date",
            "data_type": "date",
            "group_by": [ "Day", "Weekday", "Month", "Year", "YearMonth" ]
         }
      ],
      "fields": [
         {
            "class_member": "beatsPerMin",
            "long_display_name": "Beats per Minute",
            "short_display_name": "bpm",
            "supported_aggregates": [ "Min", "Max", "Count", "Sum", "Avg" ]
         },
         {
            "class_member": "source",
            "long_display_name": "Measurement Source",
            "short_display_name": "Source",
            "supported_aggregates": [ "Min", "Max", "Count", "Sum", "Avg" ]
         },
         {
            "class_member": "timestamp",
            "long_display_name": "Measurement Timestamp",
            "short_display_name": "Timestamp",
            "supported_aggregates": [ ]
         },
         {
            "class_member": "date",
            "long_display_name": "Measurement Date",
            "short_display_name": "Date",
            "supported_aggregates": [ ]
         }
      ]
   })";

} // oura_charts::schema
