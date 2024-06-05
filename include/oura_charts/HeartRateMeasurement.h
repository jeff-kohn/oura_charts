//---------------------------------------------------------------------------------------------------------------------
//
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/DataSeries.h"
#include "oura_charts/datetime_helpers.h"


namespace oura_charts
{
   /// <summary>
   ///   This class represents a heart rate data point
   /// </summary>
   class HeartRateMeasurement
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
      ///   HeartRateMeasurement constructor
      /// </summary>
      /// <remarks>
      ///   copies by default, pass rvalue-ref to move instead.
      /// </remarks>
      explicit HeartRateMeasurement(StorageType data) : m_data(std::move(data)) {}
      HeartRateMeasurement(const HeartRateMeasurement&) = default;
      HeartRateMeasurement& operator=(const HeartRateMeasurement&) = default;
      HeartRateMeasurement& operator=(HeartRateMeasurement&&) = default;
      ~HeartRateMeasurement() = default;

   private:
      StorageType m_data;
   };

   using HeartRateDataSeries = DataSeries<HeartRateMeasurement>;

   /// <summary>
   ///   custom format() support for HeartRateMeasurement
   /// </summary>
   inline auto format_as(const HeartRateMeasurement& data)
   {
      return fmt::format("[ {} HR = {}bpm at {} ]",
                         data.source(),
                         data.beatsPerMin(),
                         data.timestamp());
   }

   /// <summary>
   ///   get() overload for structured binding support for the HeartRateMeasurement class.
   /// </summary>
   template < std::size_t index > auto get(const oura_charts::HeartRateMeasurement& hr)
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
template <> struct std::tuple_size <oura_charts::HeartRateMeasurement> { static constexpr int value = 3; };
template <> struct std::tuple_element<0, oura_charts::HeartRateMeasurement> { using type = int; };
template <> struct std::tuple_element<1, oura_charts::HeartRateMeasurement> { using type = std::string; };
template <> struct std::tuple_element<2, oura_charts::HeartRateMeasurement> { using type = oura_charts::local_seconds; };

