//---------------------------------------------------------------------------------------------------------------------
//
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/DataSeries.h"
#include "oura_charts/chrono_helpers.h"


namespace oura_charts
{
   /// <summary>
   ///   This class represents a heart rate data point
   /// </summary>
   class HeartRate
   {
   public:
      using StorageType = detail::hr_data;
      static constexpr std::string_view REST_PATH = constants::REST_PATH_HEART_RATE;

      // Heart rate in BPM
      int beatsPerMin() const          {  return m_data.bpm;         }

      // Condition HR reading was taken in (awake/asleep/etc)
      std::string source() const       {  return m_data.source;     }

      // date and time (UTC) the HR reading was taken.
      local_seconds timestamp() const { return m_data.timestamp; }

      /// <summary>
      ///   HeartRate constructor
      /// </summary>
      /// <remarks>
      ///   copies by default, pass rvalue-ref to move instead.
      /// </remarks>
      explicit HeartRate(StorageType data) : m_data(std::move(data)) {}
      HeartRate(const HeartRate&) = default;
      HeartRate& operator=(const HeartRate&) = default;
      HeartRate& operator=(HeartRate&&) = default;
      ~HeartRate() = default;

   private:
      StorageType m_data;
   };

   using HeartRateDataSeries = DataSeries<HeartRate>;

   /// <summary>
   ///   custom format() support for HeartRate
   /// </summary>
   inline auto format_as(const HeartRate& data)
   {
      return fmt::format("[ {} HR = {}bpm at {} ]",
                         data.source(),
                         data.beatsPerMin(),
                         data.timestamp());
   }

   /// <summary>
   ///   get() overload for structured binding support for the HeartRate class.
   /// </summary>
   template < std::size_t index > auto get(const oura_charts::HeartRate& hr)
   {
      static_assert(index < 3);

      if constexpr (index == 0)
          return hr.beatsPerMin(); 
      else if constexpr (index == 1)
          return hr.source(); 
      else
          return hr.timestamp(); 
   }


} // namespace oura_charts

/// <summary>
///   provide a tuple-like API for class UserProfile for structured bindings:
/// </summary>
template <> struct std::tuple_size <oura_charts::HeartRate> { static constexpr int value = 3; };
template <> struct std::tuple_element<0, oura_charts::HeartRate> { using type = int; };
template <> struct std::tuple_element<1, oura_charts::HeartRate> { using type = std::string; };
template <> struct std::tuple_element<2, oura_charts::HeartRate> { using type = oura_charts::local_seconds; };

