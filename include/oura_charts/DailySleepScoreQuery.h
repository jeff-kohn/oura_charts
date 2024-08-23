#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/chrono_helpers.h"
#include "oura_charts/DailySleepScore.h"

namespace oura_charts
{

   enum class AggregateFuncType
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


   class SleepScoreQuery
   {
   public:
      enum class MemberFields
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

      struct QueryField
      {
         MemberFields   member{};
         AggregateFuncType  func{};
         std::string    title{};
      };


      SleepScoreQuery& addField(QueryField fld)
      {
         m_query_fields.push_back(std::move(fld));
         return *this;
      }


      SleepScoreQuery& addField(MemberFields member, AggregateFuncType func, std::string title)
      {
         return addField(QueryField{ member, func, std::move(title) });
      }

      void RunQuery(DailySleepScoreSeries& data_series);

   private:
      std::vector<QueryField> m_query_fields{};
   };


} // namespace oura_charts
