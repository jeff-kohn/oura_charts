//---------------------------------------------------------------------------------------------------------------------
// DataSeries.h
//
// defines the template class DataSeries<> which is use to work with data sets of various Oura Ring metrics.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/datetime_helpers.h"


namespace oura_charts
{
   template <RestNoThrowConstructable DataT, typename  Container = std::vector<DataT> >
   class DataSeries 
   {
   public:
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
   };

} // namespace oura_charts
