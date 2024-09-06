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
#include "oura_charts/functors.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/detail/aggregate_functors.h"

#include "frozen/map.h"

#include <functional>
#include <map>
#include <string>
#include <variant>


namespace oura_charts::detail
{
   // These are the potential aggregate types we may use with this class
}

namespace oura_charts
{
   /// <summary>
   ///   traits class for a HeartRate query, meant to be used for specializing a query template. This struct
   ///   contains some needed type aliases as well as functor implementations for retrieving property values from a
   ///   HeartRate object based on enumeration values. 
   /// </summary>
   struct HeartRateTraits
   {
      using RecordType    = HeartRate;
      using RecordSetType = HeartRateSeries;
      using FieldValueVt  = std::variant< int, detail::NullableInt, std::string, detail::NullableString, local_seconds, detail::Nullable<local_seconds> >;

      using AggregateSelection = detail::AggregateSelection;
      using AggregateFuncVt    = detail::AggregrateFuncVt<int, local_seconds, std::string>;


      using MemberFuncVt = std::variant< MemberSelector<HeartRate, int>,
                                         MemberSelector<HeartRate, const std::string&>,
                                         MemberSelector<HeartRate, const local_seconds&> >;

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

         return found ? mem_var->second : MemberSelector{ &HeartRate::beatsPerMin };
      }


   private:
      static constexpr auto s_property_count = std::to_underlying(PropertySelection::end);
      using PropertyMap = frozen::map<PropertySelection, MemberFuncVt, s_property_count>;

      /// <summary>
      ///   compile-time map to get a member functor from a member enum
      /// </summary>
      static inline constexpr auto s_property_map = PropertyMap{
         {PropertySelection::bpm,       MemberFuncVt{MemberSelector{&HeartRate::beatsPerMin}}},
         {PropertySelection::source,    MemberFuncVt{MemberSelector{&HeartRate::source}}},
         {PropertySelection::timestamp, MemberFuncVt{MemberSelector{&HeartRate::timestamp}}}
      };
   };

} // namespace oura_charts
