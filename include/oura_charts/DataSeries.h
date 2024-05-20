#pragma once

#include "oura_charts/oura_charts.h"
#include <vector>
#include <algorithm>


namespace oura_charts
{
   /// <summary>
   ///   template class for constructing and managing a collection of data objects from the Oura API
   /// </summary>
   /// <remarks>
   ///   this class accepts a vector of json UDT's and constructs an object for each, using move semantics
   ///   to avoid copying data. It's provides a read-only interface for working with the collection based
   ///   on std::vector
   /// </remarks>
   template <DataSeriesElement Element>
   class DataSeries : private std::vector<Element>
   {
   public:
      using container = std::vector<Element>::value_type;

      /// <summary>
      ///   DataSeries constructor. Note that data_series is passed by value, since
      ///   its elements will be moved directly into this container. To avoid
      ///   the copy, you can pass an rvalue reference if you don't need to preseve
      ///   the source data.
      /// </summary>
      DataSeries(std::vector<typename Element::StorageType> data_series)
      {
         reserve(data_series.size());
         std::for_each(std::begin(data_series.data), std::end(data_series), [this] (Element::StorageType& elem)
         {
               emplace_back(std::move(elem));
         });
      }

      using value_type = container::value_type;
      using size_type = container::size_type;
      using reference = container::reference;
      using const_reference = container::const_reference;

      using iterator = container::iterator;
      using const_iterator = container::const_iterator;
      using reverse_iterator = container::reverse_iterator;
      using const_reverse_iterator = container::const_reverse_iterator;

      using container::begin;
      using container::end;
      using container::operator[];
      using container::cdata;
   };

}
