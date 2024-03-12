//---------------------------------------------------------------------------------------------------------------------
// DataSeries.h
//
// defines the template class DataSeries<> which is use to work with data sets of various Oura Ring metrics.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/concepts.h"
#include "oura_charts/datetime_helpers.h"


namespace oura_charts
{
   template <RestNoThrowConstructable DataT, typename  Container = std::vector<DataT> >
   class DataSet 
   {
   public:

      void loadData(std::string_view path, timestamp_utc start, timestamp_utc end)
      {
      }

      //template<typename AuthType>
      //static DataSet<T> getDataSet(const AuthWrapper<AuthType>& auth, timestamp_utc begin, timestamp_utc end)
      //{
      //   retun{};
      //}

      //template<typename AuthType>
      //static DataSet<T> getDataSet(const AuthWrapper<AuthType>& auth timestamp_local begin, timestamp_local end)
      //{
      //   return getDataSet(auth, localToUtc(begin), localToUtc(end));
      //}



   private:
      Container m_data;
   };

} // namespace oura_charts
