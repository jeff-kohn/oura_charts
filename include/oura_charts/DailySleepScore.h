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

      static constexpr std::string_view REST_PATH = constants::REST_PATH_SLEEP_SESSION;

      /// <summary>
      ///   returns the id for this DailySleepScore
      /// </summary>
      const string& id() const { retrun m_data.id; }

      /// <summary>
      ///   date for the sleep score. note this date will actually be the
      ///   morning after the sleep, eg Monday's sleep score is for Sunday
      ///   night's sleep.
      /// </summary>
      chrono::year_month_day sleepDate() const { return m_data.day; }

      /// <summary>
      ///   the sleep score, a value from 1-100 taking various sleep factors
      ///   into account.
      /// </summary>
      int score() const { return m_data.score; }

      /// <summary>
      ///   map indicating the contribution of various factors to the sleeps score.
      /// </summary>
      const std::map<SleepScoreContributors, int>& contributors() const { return m_data.contributors; };

      explicit DailySleepScore(StorageType data) noexcept : m_data(std::move(data)) {}
      explicit DailySleepScore(StorageType&& data) noexcept : m_data(std::move(data)) {}
      DailySleepScore(const DailySleepScore&) = default;
      DailySleepScore(DailySleepScore&&) = default;
      ~DailySleepScore() = default;
      DailySleepScore& operator=(const DailySleepScore&) = default;
      DailySleepScore& operator=(DailySleepScore&&) = default;

   private:
      detail::daily_sleep_data m_data{};
   };


   // some useful aliaes
   using SleepScoreContributors = DailySleepScore::SleepScoreContributors;
   using DailySleepScoreSeries = DataSeries<DailySleepScore>;

   //
   // These lambda's can be used as projections for converting sessionDate() to various calandar types.
   //
   inline constexpr auto sleepScoreYearMonthDay = selectAsYearMonthDay<&DailySleepScore::sleepDate>;
   inline constexpr auto sleepScoreYear         = selectAsYear<&DailySleepScore::sleepDate>;
   inline constexpr auto sleepScoreYearMonth    = selectAsYearMonth<&DailySleepScore::sleepDate>;
   inline constexpr auto sleepScoreMonth        = selectAsMonth<&DailySleepScore::sleepDate>;
   inline constexpr auto sleepScoreWeekday      = selectAsWeekday<&DailySleepScore::sleepDate>;

   // map to group sessions by day of the week
   using SleepScoreByWeekday = MapByWeekday<DailySleepScore>;

   // map to group sessions by month of the year (not a specific year, just month)
   using SleepScoreByMonth = MapByMonth<DailySleepScore>;

   // map to group sessions by year and month
   using SleepScoreByYearMonth = MapByYearMonth<DailySleepScore>;

   // map to group sessions by year
   using SleepScoreByYear = MapByYear<DailySleepScore>;
}
