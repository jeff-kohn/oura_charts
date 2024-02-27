#pragma once
#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/rest_helpers.h"
#include "oura_charts/datetime_helpers.h"
#include <vector>
#include <string>
#include <string_view>

namespace oura_charts::heart_rate
{
   using namespace oura_charts::detail;


   /// <summary>
   ///   We store collections of HR data as
   ///
   ///      vector<expected<heart_rate::DataPoint, error>> 
   ///
   ///   so that the caller can choose how to deal with any invalid data returned.
   /// </summary>



   /// <summary>
   ///   This class represents a heart rate data point
   /// </summary>
   class DataPoint
   {
   public:
      // path used to retrieve data from REST api
      static constexpr std::string_view REST_PATH = constants::REST_PATH_HEART_RATE;

      using expected_value = expected<DataPoint, oura_exception>;
      using DataSet = std::vector<expected_value>;

      // date and time (UTC) the HR reading was taken.
      timestamp_utc timestamp() const  { return m_timestamp;   }

      // timestamp as a string in the local timezone
      std::string timestampLocal() const
      {
         auto local_tp = chrono::current_zone()->to_local(m_timestamp);
         return std::format("{:%Om/%d/%Y %r}", local_tp);
      }

      // Heart rate in BPM
      int beatsPerMin() const       { return m_bpm;         }

      // Condition HR reading was taken in (awake/asleep/etc)
      std::string source() const    { return m_source;      }

      /// <summary>
      ///   Factory method for creating a DataPoint from a JSON
      ///   document. Expected value is the requested object,
      ///   unexpected value is any error that occurred initializing
      ///   the object.
      /// </summary>
      static DataPoint::expected_value makeFromJson(const json& json_data) noexcept;

   private:
      DataPoint(int bpm, timestamp_utc timestamp, std::string source) noexcept;

      timestamp_utc  m_timestamp{};
      int            m_bpm{};
      std::string    m_source{};
   };


   /// <summary>
   ///   custom format() support for heart_rate::DataPoint
   /// </summary>
   inline auto format_as(const DataPoint& data)
   {
      return fmt::format("[ {} HR = {}bpm at {} ]",
                         data.source(),
                         data.beatsPerMin(),
                         data.timestampLocal());
   }

   
   /// <summary>
   /// Retrieve a set of requested HR data points from the Oura REST API
   /// </summary>
   template<typename AuthType>
   static DataPoint::DataSet getDataSet(const AuthWrapper<AuthType>& auth, timestamp_utc begin, timestamp_utc end)
   {
      auto begin_str = toIsoDateString(begin);
      auto end_str = toIsoDateString(end);
      return getDataSetFromRestEndpoint<DataPoint::DataSet>
               (
                  auth.getAuthorization(),
                  cpr::Parameters{ {constants::REST_PARAM_START_DATETIME, begin_str},
                                   {constants::REST_PARAM_END_DATETIME,  end_str} }
               );
   }


   /// <summary>
   /// Retrieve a set of requested HR data points from the Oura REST API
   /// </summary>
   template<typename AuthType>
   static DataPoint::DataSet getDataSet(const AuthWrapper<AuthType>& auth, timestamp_local begin , timestamp_local end)
                               
   {
      return getDataSet(auth, localToUtc(begin), localToUtc(end) );
   }

} // namespace oura_charts
