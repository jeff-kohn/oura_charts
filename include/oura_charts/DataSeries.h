#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/json_structs.h"
#include <vector>
#include <algorithm>
#include <ranges>

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
   template <DataSeriesElement ElementT>
   class DataSeries : private std::vector<ElementT>
   {
   public:
      using container = std::vector<ElementT>;

      /// <summary>
      ///   DataSeries constructor. Note that data_series is passed by value, since
      ///   its elements will be moved directly into this container. To avoid
      ///   the copy, you can pass an rvalue reference if you don't need to preserve
      ///   the source data.
      /// </summary>
       template <std::ranges::input_range RangeT>
         requires std::ranges::sized_range<RangeT> && std::same_as<rgs::range_rvalue_reference_t<RangeT>, typename ElementT::StorageType&&>
      DataSeries(RangeT data_series)
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
         return DataSeries<ElementT>( std::move(buf) );
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
      auto begin = chrono::time_point_cast<chrono::seconds>(start);
      auto end = chrono::time_point_cast<chrono::seconds>(until);
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

      return getDataSeries<ElementT>(provider, start_utc, until_utc);
   }

} // namespace oura_charts
