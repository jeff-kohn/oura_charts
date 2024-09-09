//---------------------------------------------------------------------------------------------------------------------
// DailySleepScoreTraits.h
//
// Defines the template class DailySleepScoreTraits, which is used to build queries for the DailySleepScore
// data source.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/DailySleepScore.h"
#include "oura_charts/detail/aggregate_functors.h"

#include "frozen/map.h"

#include <functional>
#include <map>
#include <variant>


namespace oura_charts
{

   /// <summary>
   ///   traits class for a DailySleepScore query, meant to be used for specializing a query template. This struct
   ///   contains some needed type aliases as well as functor implementations for retrieving property values from a
   ///   DailySleepScore object based on enumeration values. 
   /// </summary>
   class DailySleepScoreTraits
   {
   public:
      enum class PropertySelection
      {
         score,
         date,
         contrib_deep_sleep,
         contrib_efficiency,
         contrib_latency,
         contrib_rem,
         contrib_restfulness,
         contrib_sleep_timing,
         contrib_total_sleep,
         end
      };

      using AggregateSelection = detail::AggregateSelection;
      using AggregateFuncVt    = detail::AggregrateFuncVt<int, year_month_day>;
      using FieldValueVt       = std::variant<detail::NullableInt, detail::NullableDouble, detail::Nullable<year_month_day>>;
      using MemberFuncVt       = std::variant<detail::MemberSelector<DailySleepScore, int>, detail::MemberSelector<DailySleepScore, year_month_day>>;
      using RecordType         = DailySleepScore;
      using RecordSetType      = DailySleepScoreSeries;

      /// <summary>
      ///    helper function to lookup up a member functor from a member enum. This function
      ///    returns a score() MemberSelector if the requested enum wasn't found in the map
      ///    (which shouldn't be possible and indicates a bug).
      /// </summary>
      static MemberFuncVt getMember(PropertySelection member)
      {
         auto mem_var = s_property_map.find(member);
         bool found{ s_property_map.end() != mem_var }; assert(found);

         return found ? mem_var->second : detail::MemberSelector{ &DailySleepScore::score };
      }

   private:
      static constexpr auto s_property_count = std::to_underlying(PropertySelection::end);
      using PropertyMap = frozen::map<PropertySelection, MemberFuncVt, s_property_count>;

      // Map to get a member functor from a member enum
      static inline constexpr PropertyMap s_property_map
      {
         {DailySleepScoreTraits::PropertySelection::score,                MemberFuncVt{detail::MemberSelector{&DailySleepScore::score}}},
         {DailySleepScoreTraits::PropertySelection::date,                 MemberFuncVt{detail::MemberSelector{&DailySleepScore::date}}},
         {DailySleepScoreTraits::PropertySelection::contrib_deep_sleep,   MemberFuncVt{detail::MemberSelector{&DailySleepScore::contribDeepSleep}}},
         {DailySleepScoreTraits::PropertySelection::contrib_efficiency,   MemberFuncVt{detail::MemberSelector{&DailySleepScore::contribEfficiency}}},
         {DailySleepScoreTraits::PropertySelection::contrib_latency,      MemberFuncVt{detail::MemberSelector{&DailySleepScore::contribLatency}}},
         {DailySleepScoreTraits::PropertySelection::contrib_rem,          MemberFuncVt{detail::MemberSelector{&DailySleepScore::contribRemSleep}}},
         {DailySleepScoreTraits::PropertySelection::contrib_restfulness,  MemberFuncVt{detail::MemberSelector{&DailySleepScore::contribRestfulness}}},
         {DailySleepScoreTraits::PropertySelection::contrib_sleep_timing, MemberFuncVt{detail::MemberSelector{&DailySleepScore::contribTiming}}},
         {DailySleepScoreTraits::PropertySelection::contrib_total_sleep,  MemberFuncVt{detail::MemberSelector{&DailySleepScore::contribTotalSleep}}}
      };

   };

} // namespace oura_charts


