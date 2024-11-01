//---------------------------------------------------------------------------------------------------------------------
// SelectQuery.h
//
// Defines the template class AggregateQuery
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/nullable_types.h"
#include "oura_charts/QueryField.h"
#include "oura_charts/QueryFilter.h"
#include <vector>


namespace oura_charts
{


   /// <summary>
   ///   This class implements an aggregate query, that can iterate properties from a collection of objects
   ///   and compute aggregate functions on those properties.
   /// </summary>
   template <QueryTraits QueryTraitsT, template<typename> typename ContainerT = std::vector>
   class SelectQuery
   {
   public:
      using PropertySelection = QueryTraitsT::PropertySelection;   // enum specifying the object property a query field is bound to
      using RecordType        = QueryTraitsT::RecordType;          // the object type this query iterates over to collect data
      using RecordsetType     = QueryTraitsT::RecordsetType;       // the object type for a collection/series of RecordType's
      using MemberFuncVt      = QueryTraitsT::MemberFuncVt;        // variant type to hold function pointers for accessing object properties
      using FieldValueVt      = QueryTraitsT::FieldValueVt;        // default-constructible variant type to hold field values for query results.

      using Field  = QueryField<QueryTraitsT>;
      using Fields = ContainerT<Field>;

      using Filter = QueryFilter<QueryTraitsT>;
      using Filters = ContainerT<Filter>;


      /// <summary>
      ///   Add a field to the query. Calling this will clear any results currently
      ///   stored in this query object.
      /// </summary>
      void addField(Field fld){
         m_fields.push_back(std::move(fld));
      }


      /// <summary>
      ///   Add a range of fields to the query. Calling this will clear any results
      ///   currently stored in this query object.
      /// </summary>
      template<rg::input_range Rg>
         requires std::same_as<std::remove_cvref_t<rg::range_value_t<Rg>>, Field>
      void addFields(Rg&& rg)
      {
         m_fields.append_range(std::forward<Rg>(rg));
      }


      /// <summary>
      ///   Get a readonly reference to the field collection, which can be used
      ///   to retrieve values or other properties of the fields.
      /// </summary>
      /// <returns></returns>
      const Fields& fields() const {
         return m_fields;
      }


      /// <summary>
      ///   Add a filter to the query. Calling this will clear any results currently
      ///   stored in this query object. 
      /// </summary>
      void addFilter(Filter filt)
      {
         m_filters.push_back(std::move(filt));
         clearResults();
      }


      /// <summary>
      ///   add a range of filters to the query. Calling this will clear any results currently
      ///   stored in this query object. 
      /// </summary>
      template<rg::input_range Rg>
         requires std::same_as<std::remove_cvref_t<rg::range_value_t<Rg>>, Filter>
      void addFilters(Rg&& rg)
      {
         m_filters.append_range(std::forward<Rg>(rg));
         clearResults();
      }

      /// <summary>
      ///   const access to filters collection.
      /// </summary>
      /// <returns></returns>
      const Filters& filters() const { return m_filters; }

      /// <summary>
      ///   Run a query by processing the passed-in dataset and retrieving the selected
      ///   values.
      /// </summary>
      void runQuery(const RecordsetType& recordset)
      {
         // for each 'record' (object) in the 'recordset' (DataSeries),
         // we loop through all our fields and collect the needed values
         // from the object. 
         rg::for_each(getFiltered(recordset), [this] (const RecordType& rec)
                     {
                        rg::for_each(m_fields, [&rec] (Field& fld) { fld(rec); });
                     });
      }


      void clearResults()
      {
         rg::for_each(m_fields, [] (Field& fld) { fld.clear(); });
      }

   private:
      Fields  m_fields{};
      Filters m_filters{};

      auto getFiltered(const RecordsetType recordset)
      {
         return vw::filter(recordset, [this] (const RecordType& rec)
                                         {
                                            return m_filters.empty() ? true
                                                                     : rg::find_if(m_filters,  [&rec] (const Filter& filt) { return filt(rec); }) != m_filters.end();
                                         });
      }
   };



} // namespace oura_charts
