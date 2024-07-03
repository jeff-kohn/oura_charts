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
      using StorageType = detail::sleep_data;
      using SleepType = StorageType::SleepType;

      static constexpr std::string_view REST_PATH = constants::REST_PATH_SLEEP_SESSION;

      std::string sleepId() const                  {  return m_data.id;                   }
      SleepType sleepType() const                  {  return m_data.type;                 }
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
      chrono::seconds sleepTimeTotal() const       {  return m_data.total_sleep_duration; }
      chrono::seconds timeInBed() const            {  return m_data.time_in_bed;          }
                                               
      nullable_double restless_periods() const     {  return m_data.restless_periods;     }

      /// <summary>
      ///   constructor, accepts data by value, pass && to move instead of copy
      /// </summary>
      explicit SleepSession(StorageType data) : m_data(std::move(data)) {}
      SleepSession(const SleepSession&) = default;
      SleepSession(SleepSession&&) = default;
      ~SleepSession() = default;
      SleepSession& operator=(const SleepSession&) = default;
      SleepSession& operator=(SleepSession&&) = default;

   private:
      StorageType m_data;
   };

   using SleepSessionSeries = DataSeries<SleepSession>;

   
   /// <summary>
   ///   predicate to allow filtering SleepSessions by sleep type.
   ///   defaults to only selecting "long" sleep sessions.
   /// </summary>
   struct SleepTypeFilter
   {
      using SleepType = SleepSession::SleepType;

      SleepType sleep_type{ SleepType::long_sleep };

      bool operator()(const SleepSession& sess) const
      {
         return sess.sleepType() == sleep_type;
      }
   };

   static constexpr SleepTypeFilter filter_long_sleep{};
   

   // simple functor that greatly simplifies the template syntax
   // when passing a projection to range algorithms without littering
   // inline lambda's all over the place.
   struct SessionWeekday
   {
      weekday operator()(const SleepSession& sess) const
      {
         return sys_days{ sess.sessionDate() };
      }
   };

   // simple functor that greatly simplifies the template syntax
   // when passing a projection to range algorithms without littering
   // inline lambda's all over the place.
   struct SessionYearMonthDay
   {
      year_month_day operator()(const SleepSession& sess) const
      {
         return sess.sessionDate();
      }
   };

   // simple functor that greatly simplifies the template syntax
   // when passing a projection to range algorithms without littering
   // inline lambda's all over the place.
   struct SessionYearMonth
   {
      year_month operator()(const SleepSession& sess) const
      {
         return stripDay(sess.sessionDate());
      }
   };


   // simple functor that greatly simplifies the template syntax
   // when passing a projection to range algorithms without littering
   // inline lambda's all over the place.
   struct SessionYear
   {
      year operator()(const SleepSession& sess) const
      {
         return stripDayAndMonth(sess.sessionDate());
      }
   };
   

   template <typename T>
   concept SleepSessionMap =  std::same_as<std::remove_cvref<typename T::mapped_type>, SleepSession>
                           && rg::input_range<T>;


   inline WeekdayMap<SleepSession> groupByWeekday(SleepSessionSeries&& series, SleepTypeFilter filter = {})
   {
      WeekdayMap<SleepSession> sleep_by_weekday{};
      groupBy(std::move(series), sleep_by_weekday, SessionWeekday{});
      return sleep_by_weekday;
   }

   inline YearMonthMap<SleepSession> groupByYearMonth(SleepSessionSeries&& series, SleepTypeFilter filter = {})
   {
      YearMonthMap<SleepSession> sleep_by_year_month{};
      groupBy(std::move(series), sleep_by_year_month, SessionYearMonth{});
      return sleep_by_year_month;
   }

} // namesapce oura_charts
