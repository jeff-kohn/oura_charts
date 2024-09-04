//---------------------------------------------------------------------------------------------------------------------
// DailySleepScoreAggregateQuery.h
//
// Defines the template class 
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
   ///   traits class for a DailySleepScore query, meant to be used for specializing an query type. This struct
   ///   contains some needed type aliases as well as functor implementations for retrieving property values from a
   ///   DailySleepScore object based on enumeration values. 
   /// </summary>
   struct DailySleepScoreTraits
   {
      enum class PropertySelection
      {
         score,
         contrib_deep_sleep,
         contrib_efficiency,
         contrib_latency,
         contrib_rem,
         contrib_restfulness,
         contrib_sleep_timing,
         contrib_total_sleep,
         end
      };
      static constexpr auto s_property_count = std::to_underlying(PropertySelection::end);

      /// <summary>
      ///   functor to retrieve a member from the nested Contributors structs of a DailySleepScore
      /// </summary>
      struct ContribFunc
      {
         using ResultType = int;
         PropertySelection member{};
         ResultType operator()(const DailySleepScore& score) const { return getContribution(score, member); }
      };

      /// <summary>
      ///   functor to retrieve the score from a DailySleepScore object.
      /// </summary>
      struct ScoreFunc
      {
         using ResultType = int;
         ResultType operator()(const DailySleepScore& score) const
         {
            return score.score();
         }
      };

      using AggregateSelection = detail::AggregateSelection;
      using AggregateFuncVt    = detail::AggregrateFuncV<int>;
      using FieldValueVt       = std::variant<detail::NullableInt, detail::NullableDouble>;
      using MemberFuncVt       = std::variant<ScoreFunc, ContribFunc>;
      using RecordType         = DailySleepScore;
      using RecordSetType      = DailySleepScoreSeries;
      using PropertyMap        = frozen::map<PropertySelection, MemberFuncVt, s_property_count>;

      /// <summary>
      ///   Map to get a member functor from a member enum
      /// </summary>
      static inline constexpr PropertyMap s_property_map
      {
         {DailySleepScoreTraits::PropertySelection::score,                MemberFuncVt{ScoreFunc{}}},
         {DailySleepScoreTraits::PropertySelection::contrib_deep_sleep,   MemberFuncVt{ContribFunc{PropertySelection::contrib_deep_sleep}}},
         {DailySleepScoreTraits::PropertySelection::contrib_efficiency,   MemberFuncVt{ContribFunc{PropertySelection::contrib_efficiency}}},
         {DailySleepScoreTraits::PropertySelection::contrib_latency,      MemberFuncVt{ContribFunc{PropertySelection::contrib_latency}}},
         {DailySleepScoreTraits::PropertySelection::contrib_rem,          MemberFuncVt{ContribFunc{PropertySelection::contrib_rem}}},
         {DailySleepScoreTraits::PropertySelection::contrib_restfulness,  MemberFuncVt{ContribFunc{PropertySelection::contrib_restfulness}}},
         {DailySleepScoreTraits::PropertySelection::contrib_sleep_timing, MemberFuncVt{ContribFunc{PropertySelection::contrib_sleep_timing}}},
         {DailySleepScoreTraits::PropertySelection::contrib_total_sleep,  MemberFuncVt{ContribFunc{PropertySelection::contrib_total_sleep}}}
      };

      /// <summary>
      ///   Helper function to get a property from the contributors child struct of a sleep score object.
      /// </summary>
      static int getContribution(const DailySleepScore& data, PropertySelection member)
      {
         using enum PropertySelection;

         const auto& contribs = data.contributors();
         switch (member)
         {
         case contrib_deep_sleep:
            return contribs.deep_sleep;
         case contrib_efficiency:
            return contribs.efficiency;
         case contrib_latency:
            return contribs.latency;
         case contrib_rem:
            return contribs.rem_sleep;
         case contrib_restfulness:
            return contribs.restfulness;
         case contrib_sleep_timing:
            return contribs.timing;
         case contrib_total_sleep:
            return contribs.total_sleep;
         default:
            throw std::invalid_argument{ "Invalid member requested" };
         }
      }

      /// <summary>
      ///   helper function to lookup up a member functor from a member enum. Since variant<> default
      ///   initializes to its first argument, this function returns a ScoreFunc if the requested enum
      ///   wasn't found in the map (which shouldn't be possible and indicates a bug.
      /// </summary>
      static MemberFuncVt getMember(PropertySelection member)
      {
         auto mem_var = s_property_map.find(member);
         bool found{ s_property_map.end() != mem_var }; assert(found);

         return found ? MemberFuncVt{} : mem_var->second;
      }
   };


   /// <summary>
   ///   template alias for a DailySleepScore aggregate query.
   /// </summary>
   //using SleepScoreAggegateQry = AggregateQuery<DailySleepScoreQueryTraits>;

} // namespace oura_charts


