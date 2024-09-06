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
         contrib_deep_sleep,
         contrib_efficiency,
         contrib_latency,
         contrib_rem,
         contrib_restfulness,
         contrib_sleep_timing,
         contrib_total_sleep,
         end
      };

      /// <summary>
      ///   functor to retrieve a member from the nested Contributors structs of a DailySleepScore
      /// </summary>
      struct ContribFunc
      {
         using MemberType = int;
         PropertySelection member{};
         MemberType operator()(const DailySleepScore& score) const { return getContribution(score, member); }
      };

      using AggregateSelection = detail::AggregateSelection;
      using AggregateFuncVt    = detail::AggregrateFuncVt<int>;
      using FieldValueVt       = std::variant<detail::NullableInt, detail::NullableDouble>;
      using MemberFuncVt       = std::variant<detail::MemberSelector<DailySleepScore, int>, ContribFunc>;
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
         {DailySleepScoreTraits::PropertySelection::contrib_deep_sleep,   MemberFuncVt{ContribFunc{PropertySelection::contrib_deep_sleep}}},
         {DailySleepScoreTraits::PropertySelection::contrib_efficiency,   MemberFuncVt{ContribFunc{PropertySelection::contrib_efficiency}}},
         {DailySleepScoreTraits::PropertySelection::contrib_latency,      MemberFuncVt{ContribFunc{PropertySelection::contrib_latency}}},
         {DailySleepScoreTraits::PropertySelection::contrib_rem,          MemberFuncVt{ContribFunc{PropertySelection::contrib_rem}}},
         {DailySleepScoreTraits::PropertySelection::contrib_restfulness,  MemberFuncVt{ContribFunc{PropertySelection::contrib_restfulness}}},
         {DailySleepScoreTraits::PropertySelection::contrib_sleep_timing, MemberFuncVt{ContribFunc{PropertySelection::contrib_sleep_timing}}},
         {DailySleepScoreTraits::PropertySelection::contrib_total_sleep,  MemberFuncVt{ContribFunc{PropertySelection::contrib_total_sleep}}}
      };

      ///   Helper function to get a property from the contributors child struct of a sleep score object.
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

   };

} // namespace oura_charts


