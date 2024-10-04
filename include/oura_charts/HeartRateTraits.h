//---------------------------------------------------------------------------------------------------------------------
// HeartRateTraits.h
//
// Defines the template class HeartRateTraits, which is used to build queries for the HeartRate
// data source.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/aggregate_functors.h"
#include "oura_charts/detail/functors.h"
#include "oura_charts/HeartRate.h"

#include "frozen/map.h"

#include <functional>
#include <map>
#include <string>
#include <variant>


namespace oura_charts
{
   /// <summary>
   ///   traits class for a HeartRate query, meant to be used for specializing a query template. This struct
   ///   contains some needed type aliases as well as functor implementations for retrieving property values from a
   ///   HeartRate object based on enumeration values. 
   /// </summary>
   class HeartRateTraits
   {
   public:
      using RecordType    = HeartRate;
      using RecordsetType = HeartRateSeries;
      using FieldValueVt  = std::variant< int, detail::NullableInt, std::string, detail::NullableString, local_seconds, detail::Nullable<local_seconds> >;

      using AggregateSelection = detail::AggregateSelection;
      using AggregateFuncVt    = detail::AggregrateFuncVt<int, local_seconds, std::string>;


      using MemberFuncVt = std::variant< detail::MemberSelector<HeartRate, int>,
                                         detail::MemberSelector<HeartRate, const std::string&>,
                                         detail::MemberSelector<HeartRate, const local_seconds&> >;

      // Map member property enum values to functors that can retrieve those values.
      enum class PropertySelection
      {
         bpm,
         source,
         timestamp,
         end
      };

      /// <summary>
      ///   helper function to lookup up a member functor from a member enum. This function
      ///   returns a beatsPerMin() MemberSelector if the specified property enum wasn't found
      ///   in the map (which shouldn't be possible and indicates a bug).
      /// </summary>
      static MemberFuncVt getMember(PropertySelection member)
      {
         auto mem_var = s_property_map.find(member);
         bool found{ s_property_map.end() != mem_var }; assert(found);

         return found ? mem_var->second : detail::MemberSelector{ &HeartRate::beatsPerMin };
      }

   private:
      static constexpr auto s_property_count = std::to_underlying(PropertySelection::end);
      using PropertyMap = frozen::map<PropertySelection, MemberFuncVt, s_property_count>;

      // compile-time map to get a member functor from a member enum
      static inline constexpr auto s_property_map = PropertyMap{
         {PropertySelection::bpm,       MemberFuncVt{detail::MemberSelector{&HeartRate::beatsPerMin}}},
         {PropertySelection::source,    MemberFuncVt{detail::MemberSelector{&HeartRate::source}}},
         {PropertySelection::timestamp, MemberFuncVt{detail::MemberSelector{&HeartRate::timestamp}}}
      };
   };

} // namespace oura_charts
