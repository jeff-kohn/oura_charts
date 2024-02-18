#pragma once
#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/rest_helpers.h"
#include "oura_charts/detail/datetime_helpers.h"

namespace oura_charts
{
   /// <summary>
   ///   This class represents a heart rate data point
   /// </summary>
   class HeartRateDatum
   {
   public:
      HeartRateDatum(const detail::json& json_data);

      static constexpr std::string_view REST_PATH = constants::REST_PATH_HEART_RATE;

      int beatsPerMin() const       { return m_bpm;        }
      std::string source() const    { return m_source;     }

      datetime::clock::time_point timestamp() const { return m_timestamp;  }

   private:
      HeartRateDatum() = default;

      int m_bpm;
      std::string m_source{};
      datetime::clock::time_point m_timestamp{};
   };


} // namespace oura_charts
