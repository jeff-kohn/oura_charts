//---------------------------------------------------------------------------------------------------------------------
// DataSeries.h
//
// defines a template class  and related utility functions for working with a collection of data from the Oura REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/json_structs.h"
#include <algorithm>
#include <map>
#include <ranges>
#include <vector>

namespace oura_charts
{
   /// <summary>
   ///   concept for a type that be stored in a DataSeries<> collection
   /// </summary>
   template <typename T>
   concept DataSeriesElement = requires (T t, T::StorageType data)
   {
      t = T{ std::move(data) };
   };


   /// <summary>
   /// <summary>
   ///   Concept for a type that is an instantiation of the DataSeries<> template.
   /// </summary>
   template <typename T>
   concept DataSeriesObject = DataSeriesElement<typename T::ElementType> &&
                              requires (T t, T::ElementType e, T::base base_obj)
   {
      base_obj.push_back(e);
      base_obj.emplace_back(e);
   };


   /// <summary>
   ///   template class for constructing and managing a series of data objects from the Oura API
   /// </summary>
   template <DataSeriesElement ElementT>
   class DataSeries : private std::vector<ElementT>
   {
   public:
      using base = std::vector<ElementT>;
      using ElementType = ElementT;

      // expose the needed base interface from base class. clang insists on teh "typename"
      // even though I don't think it should be necessary and MSVC doens't need it.
      using typename base::value_type;
      using typename base::size_type;
      using typename base::difference_type;
      using typename base::reference;
      using typename base::const_reference;
      using typename base::pointer;
      using typename base::const_pointer;
      using typename base::reverse_iterator;
      using typename base::const_reverse_iterator;

      using base::begin;
      using base::end;
      using base::cbegin;
      using base::cend;
      using base::rbegin;
      using base::rend;
      using base::crbegin;
      using base::crend;
      using base::operator[];
      using base::size;
      using base::empty;
      using base::front;
      using base::back;


      /// <summary>
      ///   DataSeries constructor. Note that data_series is passed by value, since
      ///   its elements will be moved directly into this container. To avoid
      ///   the copy, you can pass an rvalue reference if you don't need to preserve
      ///   the source data.
      /// </summary>
      template <rg::forward_range RangeT> requires JsonStructRange<RangeT, ElementT>
      explicit DataSeries(RangeT data_series)
      {
         // stupid clang doesn't actually call ElementT(StorageType&&) like it should ,instead it default constructs and calls operator=(StorageType&&) 
         // which doesn't exist. So even though this would be faster, we have to use a ranged for loop and emplace() each element. pretty fucking lame
         // if you ask me.
         //base::insert(end(), std::make_move_iterator(data_series.begin()), std::make_move_iterator(data_series.end()));

         base::reserve(data_series.size());
         for (auto& data : data_series)
         {
            base::emplace_back(data);
         }
      }


      /// <summary>
      ///   remove all elements that don't match the supplied predicate. Unike STL
      ///   "remove" algorithms, this method actually removes the filtered elements.
      /// </summary>
      template<std::predicate<ElementType> PredT>
      size_t keepIf(PredT pred)
      {
         // we need to "negate" the predicate since we're actually specifying what
         // to remove, not what to keep, when calling remove_if()
         auto new_end = std::remove_if(begin(), end(), [pred] (const ElementType& t) -> bool { return !pred(t); });
         auto count = std::distance(new_end, end());
         base::erase(new_end, end());
         return count;
      }


      /// <summary>
      ///   remove all elements that match the supplied predicate. Unike STL
      ///   "remove" algorithms, this method actually removes the filtered elements.
      /// </summary>
      template<std::predicate<ElementType> PredT>
      size_t removeIf(PredT pred)
      {
         // we need to "negate" the predicate since we're actually specifying what
         // to remove, not what to keep, when calling remove_if()
         auto new_end = std::remove_if(begin(), end(), pred);
         auto count = std::distance(new_end, end());
         base::erase(new_end, end());
         return count;
      }


      ~DataSeries() = default;
      DataSeries(DataSeries&& other) : base(std::move(other)) {}
      DataSeries(const DataSeries& other) : base(other) {}
      DataSeries& operator=(DataSeries&& rhs)
      {
         base::operator=(std::move(rhs));
         return *this;
      }
      DataSeries& operator=(const DataSeries& rhs)
      {
         base::operator=(rhs);
         return *this;
      }
   };



   /// <summary>
   ///   Groups a data-series into a map that can accept elements grouped/sorted by the value
   ///   returned by calling the specified projection for each element in the input range.
   /// </summary>
   template<DataSeriesObject DataSeriesT, rg::range MapT, std::invocable<typename DataSeriesT::ElementType> KeyProjT>
      requires CompatibleKeyProjection<std::remove_cvref_t<MapT>, KeyProjT> 
   void groupBy(DataSeriesT&& series, MapT& map, KeyProjT&& proj)
   {
      using ValueType = DataSeriesT::ElementType;
      using MapValueType = rg::range_value_t<MapT>;

      rg::transform(std::forward<DataSeriesT>(series),
                    std::inserter(map, map.end()),
                    [&proj] (ValueType& val) -> auto
                           {
                              return MapValueType{ proj(val), std::forward<ValueType>(val) };
                           });
   }


   namespace detail
   {
      using SortedPropertyMap = std::map<std::string, std::string>;

      template <DataSeriesElement ElementT, DataProvider ProviderT, KeyValueRange MapT>
      [[nodiscard]] DataSeries<ElementT> getDataSeries(ProviderT& provider, MapT&& param_map = SortedPropertyMap{}) noexcept(false)
      {
         using JsonCollectionT = detail::RestDataCollection<typename ElementT::StorageType>;
         using CollectionBuffer = std::deque<typename JsonCollectionT::value_type>;

         // get JSON from rest server
         auto json_res = provider.getJsonData(ElementT::REST_PATH, std::forward<MapT>(param_map));
         if (!json_res)
            throw oura_exception{ json_res.error() };

         // parse into structs
         auto data_res = readJson<JsonCollectionT>(json_res.value());
         if (!data_res.has_value())
            throw oura_exception(data_res.error());

         // move the structs into temporary holding so we can accumulate if there's more.
         auto& rest_data = data_res.value();
         CollectionBuffer buf{ std::make_move_iterator(rest_data.data.begin()), std::make_move_iterator(rest_data.data.end()) };

         // as long as we got a non-null "next_token" back from the REST server, there's still more data to get.
         while (rest_data.next_token)
         {
            param_map[constants::REST_PARAM_NEXT_TOKEN] = rest_data.next_token.value();
            json_res = provider.getJsonData(ElementT::REST_PATH, std::forward<MapT>(param_map));
            if (!json_res)
               throw oura_exception{ json_res.error() };

            data_res = readJson<JsonCollectionT>(json_res.value());
            if (!data_res)
               throw oura_exception{ json_res.error() };

            rest_data = data_res.value();
            // no append_range() in libstdc++ yet
            buf.insert(buf.end(), std::make_move_iterator(rest_data.data.begin()), std::make_move_iterator(rest_data.data.end()));
         }

         // finally move the accumulated data into a new DataSeries to return
         return DataSeries<ElementT>{std::move(buf) };
      }

   } // namespace detail


   /// <summary>
   ///   Get a series of data of the requested type, for the given date range.
   /// </summary>
   /// <remarks>
   ///   This overload should be used for REST endpoints that accepts a date (no time) for the filter range.
   /// </remarks>
   template <DataSeriesElement ElementT, DataProvider ProviderT>
   [[nodiscard]] DataSeries<ElementT> getDataSeries(ProviderT& provider, chrono::year_month_day from, chrono::year_month_day thru) noexcept(false)
   {
      detail::SortedPropertyMap param_map{ { constants::REST_PARAM_START_DATE, toIsoDate(from) },
                                           { constants::REST_PARAM_END_DATE, toIsoDate(thru) } };

      return detail::getDataSeries<ElementT>(provider, std::move(param_map));
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
      detail::SortedPropertyMap param_map{ { constants::REST_PARAM_START_DATETIME, toIsoDateTime(begin) },
                                           { constants::REST_PARAM_END_DATETIME, toIsoDateTime(end) } };

      return detail::getDataSeries<ElementT>(provider, std::move(param_map));
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
