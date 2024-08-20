//---------------------------------------------------------------------------------------------------------------------7
// DailySleepScore.h
//
// Declaration for class DailySleepScore, which encapsulates a series of sleep data 
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "DataSeries.h"
#include "oura_charts/detail/json_structs.h"


namespace oura_charts
{
   class DailySleepScore
   {
   public:
      using StorageType = detail::daily_sleep_data;
      using SleepScoreContributors = StorageType::SleepScoreContributors;

      static inline constexpr std::string_view REST_PATH = constants::REST_PATH_DAILY_SLEEP;

      /// <summary>
      ///   returns the id for this DailySleepScore
      /// </summary>
      const std::string& id() const { return m_data.id; }

      /// <summary>
      ///   date for the sleep score. note this date will actually be the
      ///   morning after the sleep, eg Monday's sleep score is for Sunday
      ///   night's sleep.
      /// </summary>
      const chrono::year_month_day& date() const { return m_data.day; }

      /// <summary>
      ///   the sleep score, a value from 1-100 taking various sleep factors
      ///   into account.
      /// </summary>
      int score() const { return m_data.score; }

      /// <summary>
      ///   map indicating the contribution of various factors to the sleeps score.
      /// </summary>
      const SleepScoreContributors& contributors() const { return m_data.contributors; };

      /// <summary>
      ///   timestamp for this object
      /// </summary>
      local_seconds timestamp() const { return m_data.timestamp; }

      explicit DailySleepScore(const StorageType& data) noexcept : m_data(data) {}
      explicit DailySleepScore(StorageType&& data) noexcept : m_data(std::move(data)) {}
      DailySleepScore(const DailySleepScore&) = default;
      DailySleepScore(DailySleepScore&&) = default;
      ~DailySleepScore() = default;
      DailySleepScore& operator=(const DailySleepScore&) = default;
      DailySleepScore& operator=(DailySleepScore&&) = default;

   private:
      StorageType m_data{};
   };


   // some useful aliaes
   using SleepScoreContributors = DailySleepScore::SleepScoreContributors;
   using DailySleepScoreSeries = DataSeries<DailySleepScore>;

   //
   // These lambda's can be used as projections for converting sessionDate() to various calandar types.
   //
   inline constexpr auto sleepScoreYearMonthDay = selectAsYearMonthDay<&DailySleepScore::date>;
   inline constexpr auto sleepScoreYear         = selectAsYear<&DailySleepScore::date>;
   inline constexpr auto sleepScoreYearMonth    = selectAsYearMonth<&DailySleepScore::date>;
   inline constexpr auto sleepScoreMonth        = selectAsMonth<&DailySleepScore::date>;
   inline constexpr auto sleepScoreWeekday      = selectAsWeekday<&DailySleepScore::date>;

   //
   // aliases for grouping maps
   //
   using SleepScoreByWeekday    = MapByWeekday<DailySleepScore>;
   using SleepScoreByMonth      = MapByMonth<DailySleepScore>;
   using SleepScoreByYearMonth  = MapByYearMonth<DailySleepScore>;
   using SleepScoreByYear       = MapByYear<DailySleepScore>;


   /// <summary>
   ///   concept for a map that can hold SleepSession objects.
   /// </summary>
   template <typename T>
   concept SleepScoreMap = std::same_as<std::remove_cvref_t<typename T::mapped_type>, DailySleepScore>;


   /// <summary>
   ///   helper function for efficiently using the groupBy() functionality of DataSeries object for sleep scores.
   /// </summary>
   template <SleepScoreMap MapT, std::invocable<DailySleepScore> KeyProjT>  requires CompatibleKeyProjection <MapT, KeyProjT>
   inline auto group(DailySleepScoreSeries&& series, KeyProjT&& proj)
   {
      MapT map{};
      groupBy(std::move(series), map, proj);
      return map;
   }

   /// <summary>
   ///   get() overload for structured binding support for the DailySleepScore class.
   /// </summary>
   template < std::size_t index > auto get(const oura_charts::DailySleepScore& score)
   {
      static_assert(index < 5);

      if constexpr (index == 0)
      {
         return score.id();
      }
      else if constexpr (index == 1)
      {
         return score.date();
      }
      else if constexpr (index == 2)
      {
         return score.score();
      }
      else if constexpr (index == 3)
      {
         return score.contributors();
      }
      else // 4
      {
         return score.timestamp();
      }
   }


   enum class AggregateFunc
   {
      Min,
      Max,
      Count,
      Sum,
      Avg
   };


   enum class DateGrouping
   {
      Day,
      Weekday,
      Month,
      Year,
      YearMonth
   };

   template <SleepScoreMap MapT>
   class SleepScoreQuery
   {
   public:
      enum class Members
      {
         score,
         date,
         contrib_deep_sleep,
         contrib_efficiency,
         contrib_latency,
         contrib_rem,
         contrib_restfulness,
         contrib_sleep_timing,
         contrib_total_sleep
      };

      class QueryField
      {
         Members member{ Members::score };
         AggregateFunc func{};
         std::string title{};
      };


      SleepScoreQuery& setGroupBy(DateGrouping group_by)
      {
         m_group_by = group_by;
         return *this;
      }

      DateGrouping getGroupBy() const
      {
         return m_group_by;
      }

      SleepScoreQuery& addField(QueryField fld)
      {
         m_query_fields.push_back(std::move(fld));
         return *this;
      }

      SleepScoreQuery& addField(Members member, AggregateFunc func, std::string_view title)
      {
         return addField({ member, func, title });
      }

   private:
      DateGrouping            m_group_by{};
      std::vector<QueryField> m_query_fields{};
   };




}

/// <summary>
///   provide a tuple-like API for class DailySleepScore for structured bindings:
/// </summary>
template <> struct std::tuple_size <oura_charts::DailySleepScore> { static inline constexpr int value = 5; };
template <> struct std::tuple_element<0, oura_charts::DailySleepScore> { using type = std::string; };
template <> struct std::tuple_element<1, oura_charts::DailySleepScore> { using type = oura_charts::year_month_day; };
template <> struct std::tuple_element<2, oura_charts::DailySleepScore> { using type = int; };
template <> struct std::tuple_element<3, oura_charts::DailySleepScore> { using type = oura_charts::SleepScoreContributors; };
template <> struct std::tuple_element<4, oura_charts::DailySleepScore> { using type = oura_charts::local_seconds; };
