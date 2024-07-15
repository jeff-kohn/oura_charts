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
      chrono::year_month_day date() const { return m_data.day; }

      /// <summary>
      ///   the sleep score, a value from 1-100 taking various sleep factors
      ///   into account.
      /// </summary>
      int score() const { return m_data.score; }

      /// <summary>
      ///   map indicating the contribution of various factors to the sleeps score.
      /// </summary>
      const SleepScoreContributors& contributors() const { return m_data.contributors; };

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


   template <SleepScoreMap MapT, std::invocable<DailySleepScore> KeyProjT>  requires CompatibleKeyProjection <MapT, KeyProjT>
   inline auto group(DailySleepScoreSeries&& series, KeyProjT&& proj)
   {
      MapT map{};
      groupBy(std::move(series), map, proj);
      return map;
   }
}
