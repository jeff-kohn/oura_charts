//---------------------------------------------------------------------------------------------------------------------
// QueryField.h
//
// Defines the template class AggregateQuery
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/nullable_types.h"

#include <deque>


namespace oura_charts
{
   
   /// <summary>
   ///   concept for a traits class that contains the need members to specialize a QueryField<> template
   /// </summary>
   template<typename T>
   concept QueryTraits = requires (T t, T::PropertySelection sel, T::MemberFuncVt mem_func)
   {
      typename T::PropertySelection;
      typename T::RecordType;
      typename T::MemberFuncVt;
      typename T::FieldValueVt;

      mem_func = T::getMember(sel);
   };


   template<QueryTraits QueryTraitsT, template<typename> typename ContainerT = std::deque>
   class QueryField
   {
   public:
      using PropertySelection = QueryTraitsT::PropertySelection;   // enum specifying the object property a query field is bound to
      using RecordType        = QueryTraitsT::RecordType;          // the object type this query iterates over to collect data
      using MemberFuncVt      = QueryTraitsT::MemberFuncVt;        // variant type to hold function pointers for accessing object properties
      using FieldValueVt      = QueryTraitsT::FieldValueVt;        // default-constructible variant type to hold field values for query results.
      using ValArray          = ContainerT<FieldValueVt>;

      explicit QueryField(PropertySelection prop = {})
      {
         setProperty(prop);
      }
      QueryField(const QueryField&) = default;
      QueryField(QueryField&&) noexcept = default;
      QueryField& operator=(const QueryField&) = default;
      QueryField& operator=(QueryField&&) noexcept = default;
      ~QueryField() = default;


      /// <summary>
      ///   provides const access to the array of collected values.
      /// </summary>
      /// <returns></returns>
      const ValArray& values() const
      {
         return m_values;
      }


      /// <summary>
      ///   clear any previously collected values.
      /// </summary>
      void clear()
      {
         m_values.clear();
      }


      /// <summary>
      ///   get/set the object property to use for this field. calling this will discard any data
      ///   currently collected by this field (eg values() will be empty.
      /// </summary>
      PropertySelection getProperty() const noexcept
      {
         return m_prop;
      }
      QueryField& setProperty(PropertySelection prop)
      {
         // update member selector
         m_prop = prop;
         m_member_func = QueryTraitsT::getMember(m_prop);
         m_values.clear();
         return *this;
      }


      /// <summary>
      ///   process an input 'record', storing the requested FieldValueVt
      /// </summary>
      void operator()(const RecordType& rec)
      {
         assert(m_member_func.has_value());

         return std::visit([] (auto&& func)
                           {
                              return FieldValueVt(func.result());
                           },
                           *m_member_func);
      }

   private:
      PropertySelection              m_prop{};
      ValArray                       m_values{};
      detail::Nullable<MemberFuncVt> m_member_func{}; // only using optional<> because MemberSelector<> isn't default constructible. But this
                                                      // variable will always contain a valid functor since setProperty() is called from ctor

   };



} // namespace oura_charts
