//---------------------------------------------------------------------------------------------------------------------7
// SleepDataSeries.h
//
// Declaration for class SleepDataSeriers, which encapsulates a series of sleep data 
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "DataSeries.h"
#include "oura_charts/detail/json_structs.h"

namespace oura_charts
{
   using detail::nullable_double;


   /// <summary>
   ///   encapsulates data for a single sleep session from the Oura API.
   /// </summary>
   class SleepSession
   {
   public:
      // Required by DataSeriesElement concept
      using StorageType = detail::sleep_data;
      static constexpr std::string_view REST_PATH = constants::REST_PATH_SLEEP_SESSION;


      /// <summary>
      ///   constructor, accepts data by value, pass && to move instead of copy
      /// </summary>
      explicit SleepSession(StorageType data) : m_data(std::move(data)) {}
      ~SleepSession() = default;

      // default copy/move semantics
      SleepSession(const SleepSession&) = default;
      SleepSession(SleepSession&&) = default;
      SleepSession& operator=(const SleepSession&) = default;
      SleepSession& operator=(SleepSession&&) = default;

      std::string sleepId() const                  {  return m_data.id;                   }
      chrono::year_month_day sessionDate() const   {  return m_data.day;                  }
      local_seconds bedtimeStart() const           {  return m_data.bedtime_start;        }
      local_seconds bedtimeEnd() const             {  return m_data.bedtime_end;          }

      nullable_double avgBreathingRate() const     {  return m_data.average_breath;       }
      nullable_double avgHeartRate() const         {  return m_data.average_heart_rate;   }
      nullable_double avgHRV() const               {  return m_data.average_hrv;          }
      nullable_double restingHeartRate() const     {  return m_data.lowest_heart_rate;    }

      chrono::seconds latency() const              {  return m_data.latency;              }
      chrono::seconds timeAwake() const            {  return m_data.awake_time;           }
      chrono::seconds sleepTimeDeep() const        {  return m_data.deep_sleep_duration;  }
      chrono::seconds sleepTimeLight() const       {  return m_data.light_sleep_duration; }
      chrono::seconds sleepTimeREM() const         {  return m_data.rem_sleep_duration;   }
                                               
      nullable_double restless_periods() const     {  return m_data.restless_periods;     }

   private:
      StorageType m_data;
   };

   /// <summary>
   ///   a collection of SleepSession objects
   /// </summary>
   using SleepSessionSeries = DataSeries<SleepSession>;


} // namesapce oura_charts
