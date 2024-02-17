#pragma once
#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/rest_helpers.h"

namespace oura_charts
{
   namespace detail
   {
      struct hr_data
      {
         int bpm;
         std::string source;
         std::string timestamp;
      };
   }

   /// <summary>
   ///   This class represents a heart rate data point
   /// </summary>
   class HeartRateDatum : private detail::hr_data
   {
   public:
      HeartRateDatum(const detail::json& json_data);

      static constexpr std::string_view REST_PATH = constants::REST_PATH_HEART_RATE;

      int beatsPerMin() const       { return hr_data::bpm;        }
      std::string source() const    { return hr_data::source;     }
      std::string timestamp() const { return hr_data::timestamp;  }

   private:
      HeartRateDatum() = default;
   };


} // namespace oura_charts
