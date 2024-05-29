#pragma once

#include "oura_charts/oura_charts.h"
#include <vector>
#include <algorithm>
#include <string_view>

namespace oura_charts
{
   /// <summary>
   ///   template class for constructing and managing a collection of data objects from the Oura API
   /// </summary>
   /// <remarks>
   ///   this class accepts a vector of json UDT's and constructs an object for each, using move semantics
   ///   to avoid copying data. The collection is considered static once initialized, so operations that modify
   ///   its contents are not provided but the usual element access is.
   /// </remarks>
   template <DataSeriesElement Element>
   class DataSeries : private std::vector<Element>
   {
   public:
      using container = std::vector<Element>;

      /// <summary>
      ///   DataSeries constructor. Note that data_series is passed by value, since
      ///   its elements will be moved directly into this container. To avoid
      ///   the copy, you can pass an rvalue reference if you don't need to preseve
      ///   the source data.
      /// </summary>
      /// <todo>
      ///   use a range template param instead of hard-coded vector.
      /// </todo>
      DataSeries(std::vector<typename Element::StorageType> data_series)
      {
         container::reserve(data_series.size());
         for (auto&& elem : data_series)
         {
            container::emplace_back(std::move(elem));
         };
      }
      ~DataSeries() = default;

      // move/copy construction and assignment
      DataSeries(DataSeries&& other) : container(std::move(other)) {}
      DataSeries(const DataSeries& other) : container(other) {}
      DataSeries& operator=(DataSeries&& rhs)
      {
         container::operator=(std::move(rhs));
         return *this;
      }
      DataSeries& operator=(const DataSeries& rhs)
      {
         container::operator=(rhs);
         return *this;
      }

      // expose the needed container interface from base class.
      using container::begin;
      using container::end;
      using container::cbegin;
      using container::cend;
      using container::rbegin;
      using container::rend;
      using container::crbegin;
      using container::crend;
      using container::operator[];
      using container::size;
      using container::empty;
      using container::front;
      using container::back;

   };

   template <DataSeriesElement ElementT, DataProvider ProviderT>
   [[nodiscard]] DataSeries<ElementT> getDataSeries(ProviderT& provider, utc_timestamp start, utc_timestamp end) noexcept(false)
   {
      using JsonCollectionT = detail::RestDataCollection<typename ElementT::StorageType>;

      auto exp_json = provider.getJsonDataSeries(ElementT::REST_PATH, start, end);
      if (!exp_json)
         throw exp_json.error();

      auto exp_data = detail::readJson<JsonCollectionT>(exp_json.value());
      if (!exp_data)
         throw exp_json.error();

      auto&& rest_data = exp_data.value();
      // todo: handle data-paging with next_token.
      return DataSeries<ElementT>(std::move(rest_data.data));
   }

   template <DataSeriesElement ElementT, DataProvider ProviderT>
   DataSeries<ElementT> getDataSeries(ProviderT& provider, local_timestamp start, local_timestamp end)
   {
      return getDataSeries<ElementT, ProviderT>(provider, localToUtc(start), localToUtc(end));
   }


}
