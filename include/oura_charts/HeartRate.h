//---------------------------------------------------------------------------------------------------------------------
//
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/datetime_helpers.h"
#include <vector>
#include <string>
#include <string_view>

namespace oura_charts
{
   using namespace oura_charts::detail;


   /// <summary>
   ///   This class represents a heart rate data point
   /// </summary>
   class HeartRateMeasurement
   {
   public:
      // path used to retrieve data from REST api
      static constexpr std::string_view REST_PATH = constants::REST_PATH_HEART_RATE;

      // return type used by no-throw factory method.
      using expected_object = expected<HeartRateMeasurement, oura_exception>;

      /// <summary>
      ///   Factory method for creating a DataPoint from a JSON
      ///   document. Expected value is the requested object,
      ///   unexpected value is any error that occurred initializing
      ///   the object.
      /// </summary>
      static expected_object makeFromJson(const json& json_data) noexcept;


      // date and time (UTC) the HR reading was taken.
      timestamp_utc timestamp() const  { return m_timestamp;   }

      // Heart rate in BPM
      int beatsPerMin() const       { return m_bpm;         }

      // Condition HR reading was taken in (awake/asleep/etc)
      std::string source() const    { return m_source;      }

   private:
      HeartRateMeasurement(int bpm, timestamp_utc timestamp, std::string source) noexcept;

      timestamp_utc  m_timestamp{};
      int            m_bpm{};
      std::string    m_source{};
   };


   /// <summary>
   ///   custom format() support for heart_rate::DataPoint
   /// </summary>
   //inline auto format_as(const DataPoint& data)
   //{
   //   return fmt::format("[ {} HR = {}bpm at {} ]",
   //                      data.source(),
   //                      data.beatsPerMin(),
   //                      data.timestampLocal());
   //}

   //
   ///// <summary>
   ///// Retrieve a set of requested HR data points from the Oura REST API
   ///// </summary>
   //template<typename AuthType>
   //static DataPoint::DataSet getDataSet(const AuthWrapper<AuthType>& auth, utc_timestamp begin, utc_timestamp end)
   //{
   //   auto begin_str = toIsoDateTime(begin);
   //   auto end_str = toIsoDateTime(end);
   //   return getDataSetFromRestEndpoint<DataPoint::DataSet>
   //            (
   //               auth.getAuthorization(),
   //               cpr::Parameters{ {constants::REST_PARAM_START_DATETIME, begin_str},
   //                                {constants::REST_PARAM_END_DATETIME,  end_str} }
   //            );
   //}


   ///// <summary>
   ///// Retrieve a set of requested HR data points from the Oura REST API
   ///// </summary>
   //template<typename AuthType>
   //static DataPoint::DataSet getDataSet(const AuthWrapper<AuthType>& auth, local_timestamp begin , local_timestamp end)
   //                            
   //{
   //   return getDataSet(auth, localToUtc(begin), localToUtc(end) );
   //}

} // namespace oura_charts
