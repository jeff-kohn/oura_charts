#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/json_structs.h"
#include <vector>
#include <algorithm>

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

   namespace detail
   {
      template <DataSeriesElement ElementT, DataProvider ProviderT, KeyValueRange MapT>
      [[nodiscard]] DataSeries<ElementT> getDataSeries(ProviderT& provider, MapT param_map) noexcept(false)
      {
         using JsonCollectionT = detail::RestDataCollection<typename ElementT::StorageType>;
         using CollectionBuffer = std::deque<typename JsonCollectionT::value_type>;

         // get JSON from rest server
         auto&& exp_json = provider.getJsonDataSeries(ElementT::REST_PATH, param_map);
         if (!exp_json)
            throw exp_json.error();

         // parse into structs
         auto&& exp_data = readJson<JsonCollectionT>(exp_json.value());
         if (!exp_data)
            throw exp_json.error();

         // move the structs into temporary holding so we can accumulate if there's more.
         auto&& rest_data = exp_data.value();
         CollectionBuffer buf{ std::make_move_iterator(rest_data.data.begin()), std::make_move_iterator(rest_data.data.end()) };

         // as long as we got a non-null "next_token" back from the REST server, there's still more data to get.
         while (rest_data.next_token)
         {
            param_map[constants::REST_PARAM_NEXT_TOKEN] = rest_data.next_token.value();
            exp_json = provider.getJsonDataSeries(ElementT::REST_PATH, param_map);
            if (!exp_json)
               throw exp_json.error();

            exp_data = readJson<JsonCollectionT>(exp_json.value());
            if (!exp_data)
               throw exp_json.error();

            rest_data = exp_data.value();
            buf.append_range(std::move(rest_data.data));
         }

         // finally move the accumulated data into a new DataSeries to return 
         return DataSeries<ElementT>(std::move(rest_data.data));
      }

   } // namespace detail


   /// <summary>
   ///   Get a series of data of the requested type, for the given date range.
   /// </summary>
   /// <remarks>
   ///   This overload should be used for REST endpoints that accepts a date (no time) for the filter.
   /// </remarks>
   template <DataSeriesElement ElementT, DataProvider ProviderT>
   [[nodiscard]] DataSeries<ElementT> getDataSeries(ProviderT& provider, chrono::year_month_day from, chrono::year_month_day thru) noexcept(false)
   {
      std::unordered_map<std::string, std::string> param_map{ { constants::REST_PARAM_START_DATE, toIsoDate(from) },
                                                              { constants::REST_PARAM_END_DATE, toIsoDate(thru) } };

      return detail::getDataSeries<ElementT>(provider, param_map);
   }

   /// <summary>
   ///   Get a series of data of the requested type, for the given time period.
   /// </summary>
   /// <remarks>
   ///   This overload should be used for REST endpoints that accepts a date/time value for the filter.
   /// </remarks>
   template <DataSeriesElement ElementT, DataProvider ProviderT, typename DurationT>
   [[nodiscard]] DataSeries<ElementT> getDataSeries(ProviderT& provider, chrono::sys_time<DurationT> start, chrono::sys_time<DurationT> until) noexcept(false)
   {
      auto begin = chrono::time_point_cast<chrono::sys_seconds>(start);
      auto end = chrono::time_point_cast<chrono::sys_seconds>(until);
      std::unordered_map<std::string, std::string> param_map{ { constants::REST_PARAM_START_DATETIME, toIsoDateTime(begin) },
                                                              { constants::REST_PARAM_END_DATETIME, toIsoDateTime(end) } };

      return detail::getDataSeries<ElementT>(provider, param_map);
   }


   /// <summary>
   ///   Get a series of data of the requested type, for the given time period.
   /// </summary>
   /// <remarks>
   ///   This overload converts local time to UTC, and should be used for REST endpoints that accept
   ///   a date/time value for the filter rather than just a date.
   /// </remarks>
   template <DataSeriesElement ElementT, DataProvider ProviderT, typename DurationT>
   [[nodiscard]] DataSeries<ElementT> getDataSeries(ProviderT& provider, chrono::local_time<DurationT> start, chrono::local_time<DurationT> until) noexcept(false)
   {
      auto start_utc = localToUtc(start);
      auto until_utc = localToUtc(until);

      return getDataSeries<ElementT>(provider, start, until);
   }

} // namespace oura_charts
