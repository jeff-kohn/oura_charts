//---------------------------------------------------------------------------------------------------------------------7
// SleepSession.h
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
   /// <summary>
   ///   encapsulates data for a single sleep session from the Oura API.
   /// </summary>
   class SleepSession
   {
   public:
      using NullableDouble = detail::NullableDouble;
      using NullableUInt = detail::NullableUInt;
      using StorageType = detail::sleep_data;
      using ReadinessContributors = StorageType::ReadinessContributors;
      using ReadinessContributors = StorageType::ReadinessContributors;
      using SleepType = StorageType::SleepType;

      static inline constexpr std::string_view REST_PATH = constants::REST_PATH_SLEEP_SESSION;

      const std::string& sleepId() const                {  return m_data.id;                   }
      SleepType sleepType() const                       {  return m_data.type;                 }
      const chrono::year_month_day& sessionDate() const {  return m_data.day;                  }
      const local_seconds& bedtimeStart() const         {  return m_data.bedtime_start;        }
      const local_seconds& bedtimeEnd() const           {  return m_data.bedtime_end;          }

      const NullableDouble& avgBreathingRate() const    {  return m_data.average_breath;       }
      const NullableDouble& avgHeartRate() const        {  return m_data.average_heart_rate;   }
      const NullableDouble& avgHRV() const              {  return m_data.average_hrv;          }
      const NullableUInt& restingHeartRate() const      {  return m_data.lowest_heart_rate;    }

      const chrono::seconds& latency() const            {  return m_data.latency;              }
      const chrono::seconds& timeAwake() const          {  return m_data.awake_time;           }
      const chrono::seconds& sleepTimeDeep() const      {  return m_data.deep_sleep_duration;  }
      const chrono::seconds& sleepTimeLight() const     {  return m_data.light_sleep_duration; }
      const chrono::seconds& sleepTimeREM() const       {  return m_data.rem_sleep_duration;   }
      const chrono::seconds& sleepTimeTotal() const     {  return m_data.total_sleep_duration; }
      const chrono::seconds& timeInBed() const          {  return m_data.time_in_bed;          }
                                               
      const NullableUInt& restlessPeriods() const       {  return m_data.restless_periods;     }

      /// <summary>
      ///   constructor accepts data by value, pass && to move instead of copy
      /// </summary>
      explicit SleepSession(StorageType data) noexcept : m_data(std::move(data)) {}
      SleepSession(const SleepSession&) = default;
      SleepSession(SleepSession&&) = default;
      ~SleepSession() = default;
      SleepSession& operator=(const SleepSession&) = default;
      SleepSession& operator=(SleepSession&&) = default;

   private:
      StorageType m_data;
   };

   using SleepType = SleepSession::SleepType;
   using ReadinessContributors = SleepSession::ReadinessContributors;
   using SleepSessionSeries = DataSeries<SleepSession>;


   /// <summary>
   ///   predicate to allow filtering SleepSessions by sleep type.
   /// </summary>
   struct SleepTypeFilter
   {
      // default value of "any" will match all.
      SleepType sleep_type{ SleepType::any };

      constexpr bool operator()(const SleepSession& sess) const
      {
         return sleep_type == SleepType::any ? true
                                             : (sess.sleepType() == sleep_type);
      }
   };


   /// <summary>
   ///   unary predicates to filter SleepSession objects based on sleep type
   /// </summary>
   inline constexpr SleepTypeFilter long_sleep_filter{SleepType::long_sleep};


   //
   // These lambda's can be used as projections for converting sessionDate() to various calandar types.
   //
   inline constexpr auto sessionYearMonthDay = selectAsYearMonthDay<&SleepSession::sessionDate>;
   inline constexpr auto sessionYear         = selectAsYear<&SleepSession::sessionDate>;
   inline constexpr auto sessionYearMonth    = selectAsYearMonth<&SleepSession::sessionDate>;
   inline constexpr auto sessionMonth        = selectAsMonth<&SleepSession::sessionDate>;
   inline constexpr auto sessionWeekday      = selectAsWeekday<&SleepSession::sessionDate>;

    
   //
   // aliases for grouping maps
   //
   using SleepByWeekday   = MapByWeekday<SleepSession>;
   using SleepByMonth     = MapByMonth<SleepSession>;
   using SleepByYearMonth = MapByYearMonth<SleepSession>;
   using SleepByYear      = MapByYear<SleepSession>;


   /// <summary>
   ///   concept for a map that can hold SleepSession objects.
   /// </summary>
   template <typename T>
   concept SleepSessionMap = std::same_as<std::remove_cvref_t<typename T::mapped_type>, SleepSession>;


   template <SleepSessionMap MapT, std::invocable<SleepSession> KeyProjT>  requires CompatibleKeyProjection <MapT, KeyProjT>
   inline auto group(SleepSessionSeries&& series, KeyProjT&& proj)
   {
      MapT map{};
      groupBy(std::move(series), map, proj);
      return map;
   }


   /// <summary>
   ///   group a SleepSession series by projecting the sessionDate to the
   ///   desired grouping value (day of week, month of year, etc). This
   ///   is an eager/greedy algorithm, elements are MOVED from the source
   ///   container to the target map.
   /// </summary>
   template <SleepSessionMap MapT, std::invocable<SleepSession> KeyProjT>  requires CompatibleKeyProjection <MapT, KeyProjT>
   inline auto group(SleepSessionSeries&& series, KeyProjT proj, SleepTypeFilter filter)
   {
      // First filter out the values we don't want.
      series.keepIf(filter);
      return group<MapT>(std::move(series), proj);
   }

} // namesapce oura_charts
