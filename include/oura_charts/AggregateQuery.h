//---------------------------------------------------------------------------------------------------------------------
// AggregateQuery.h
//
// Defines the template class AggregateQuery
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/aggregate_functors.h"
#include "oura_charts/detail/nullable_types.h"

#include <vector>


namespace oura_charts
{
   /// <summary>
   ///   concept for a type that can be derived from AggregateQuery<> to implement a query class for one of the
   ///   Oura data sources.
   /// </summary>
   template<typename T>
   concept AggregateQueryTraits = requires (T t, T::PropertySelection sel, T::MemberFuncVt mem_func, T::AggregateFuncVt a_func, T::FieldValueVt fld_val)
   {
      typename T::PropertySelection;
      typename T::AggregateSelection;
      typename T::RecordType;
      typename T::RecordsetType;
      typename T::MemberFuncVt;
      typename T::AggregateFuncVt;
      typename T::FieldValueVt;

      mem_func = T::getMember(sel);
      //fld_val = func(mem_func(t));
   };


   /// <summary>
   ///   This class implements an aggregate query, that can iterate properties from a collection of objects
   ///   and compute aggregate functions on those properties.
   /// </summary>
   template <AggregateQueryTraits QueryTraitsT, template<typename> typename ContainerT = std::vector>
   class AggregateQuery
   {
   public:
      using PropertySelection  = QueryTraitsT::PropertySelection;   // enum specifying the object property a query field is bound to
      using AggregateSelection = QueryTraitsT::AggregateSelection;  // enum specifying the aggregate function a query field is bound to
      using RecordType         = QueryTraitsT::RecordType;          // the object type this query iterates over to collect data
      using RecordsetType      = QueryTraitsT::RecordsetType;       // the object type for a collection/series of RecordType's
      using MemberFuncVt       = QueryTraitsT::MemberFuncVt;        // variant type to hold function pointers for accessing object properties
      using AggregateFuncVt    = QueryTraitsT::AggregateFuncVt;     // default-constructible variant type to hold function pointers for computing aggregate functions
      using FieldValueVt       = QueryTraitsT::FieldValueVt;        // default-constructible variant type to hold field values for query results.

      class Field
      {
      public:
         explicit Field(PropertySelection prop, AggregateSelection aggregate = {})
         {
            setProperty(prop);
            setAggregate(aggregate);
         }
         Field(const Field&) = default;
         Field(Field&&) noexcept = default;
         Field& operator=(const Field&) = default;
         Field& operator=(Field&&) noexcept = default;
         ~Field() = default;


         /// <summary>
         ///   get/set the object property to use for this field.
         /// </summary>
         PropertySelection getProperty() const noexcept
         {
            return m_prop;
         }
         Field& setProperty(PropertySelection prop)
         {
            // update member selector
            m_prop = prop;
            m_member_func = QueryTraitsT::getMember(m_prop);

            // also update aggregate func, because the member type may have changed.
            setAggregate(m_aggregate);

            return *this;
         }

         /// <summary>
         ///   get/set the aggregate function for this query field.
         ///   setting the aggregate will cause
         /// </summary>
         AggregateSelection getAggregate() const noexcept
         {
            return m_aggregate;
         }
         Field& setAggregate(AggregateSelection aggregate)
         {
            assert(m_member_func.has_value());

            // we don't check for setting the same aggregate, because even if it's the same
            // value/type we want a new/clean instance when getting called from clearResult().
            m_aggregate = aggregate;
            m_aggregate_func = std::visit([this] <typename FuncT> ([[maybe_unused]] FuncT& mem_fn) -> AggregateFuncVt
                                          {
                                             return detail::getAggregateFunctor<AggregateFuncVt, typename FuncT::MemberType>(m_aggregate);
                                          },
                                          *m_member_func);
            return *this;
         }

         /// <summary>
         ///   process an input 'record'.
         /// </summary>
         void operator()(const RecordType& rec)
         {
            assert(m_member_func.has_value());

            auto prop_val = std::visit([&rec] (auto&& func)
                            {
                               return func(rec);
                            },
                            *m_member_func);

            // cppcheck-suppress constParameterReference
            //std::visit([&rec, this](auto& aggr_func) -> auto
            //           {
            //              aggr_func(std::visit([&rec](auto& mem_func) -> auto
            //                                 {
            //                                    return mem_func(rec);
            //                                 },
            //                                 *m_member_func));
            //           },
            //           m_aggregate_func);
         }

         /// <summary>
         ///   returns the result value for this query field, which
         ///   may be std::nullopt
         /// </summary>
         FieldValueVt getResult() const
         {
            return std::visit([] (auto&& func)
                              {
                                 auto val = func.result();
                                 return FieldValueVt(val);
                              },
                              m_aggregate_func);
         }

         /// <summary>
         ///   reset the state of this object, removing any accumulated results.
         /// </summary>
         void clearResult()
         {
            // just reset the functor so we get a fresh object.
            setAggregate(m_aggregate);
         }

      private:
         AggregateSelection     m_aggregate{};
         AggregateFuncVt        m_aggregate_func{};
         PropertySelection      m_prop{};
         detail::Nullable<MemberFuncVt> m_member_func{}; // only using optional<> because MemberSelector<> isn't default constructible. But this
                                                         // variable will always contain a valid functor since setProperty() is called from ctor
      };

      // container type we use for storing our query fields
      using QueryFields = ContainerT<Field>;

      /// <summary>
      ///   returns reference to the fields array for this query, which can be used
      ///   to configure the fields or retrieve their values
      /// </summary>
      QueryFields m_fields{};

      /// <summary>
      ///   Runs this query on the supplied recordset. After completion, the aggregate
      ///   result values can be retrieved through the fields() property.
      /// </summary>
      /// <remarks>
      ///   Note that the aggregate functors' states are not reset when a query is run,
      ///   so you can aggregate multiple recordsets by calling runQuery() multiple times.
      ///   If you want to re-use a query object to run a fresh query, you should call
      ///   clearResults() first.
      /// </remarks>
      void runQuery(const RecordsetType& recordset)
      {
         // this functor will get called for each 'record' (object)
         // in the 'recordset' (array). It loops through
         // all our fields and collects the values from the
         // object, passing them to the functors.
         auto row_func = [this](const RecordType& rec)
            {
               rg::for_each(m_fields, [&rec] (Field& fld) { fld(rec); });
            };

         rg::for_each(recordset, row_func);
      }

      /// <summary>
      ///   clear any existing results from a previous runQuery call.
      /// </summary>
      void clearResults()
      {
         rg::for_each(m_fields, [] (auto&& fld) { fld.clearResult(); });
      }
   };


} // namespace oura_charts
