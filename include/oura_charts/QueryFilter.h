//---------------------------------------------------------------------------------------------------------------------
// QueryFilter.h
//
// Defines the template class QueryFilter
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/QueryField.h"

#include <variant>


namespace oura_charts
{
   template <class QueryTraitsT>
   class QueryFilter
   {
   public:
      using PropertySelection = QueryTraitsT::PropertySelection; // enum specifying the object property a query field is bound to
      using RecordType        = QueryTraitsT::RecordType;        // the object type this query iterates over to collect data
      using RecordsetType     = QueryTraitsT::RecordsetType;     // the object type for a collection/series of RecordType's
      using MemberFuncVt      = QueryTraitsT::MemberFuncVt;      // variant type to hold function pointers for accessing object properties
      using FieldValueVt      = QueryTraitsT::FieldValueVt;      // default-constructible variant type to hold field values for query results.

      using FilterPredicate = std::function<bool(const FieldValueVt&)>;

      QueryFilter(PropertySelection sel, FilterPredicate pred) :
         mem_func{ QueryTraitsT::getMember(sel) },
         m_pred{ pred }
      {
      }

      bool operator()(const RecordType& rec)
      {
         // Get a variant containing the member value.
         auto val_vt = std::visit([&rec] (auto&& func){
            return func(rec);
         },
         mem_func);

         return pred(val_vt);
      }

   private:
      MemberFuncVt    mem_func{};
      FilterPredicate m_pred{};

   };

} // namespace oura_charts
