#include "oura_charts/HeartRate.h"

namespace oura_charts::detail
{
   // POD used for JSON binding.
   struct hr_data
   {
      int bpm;
      std::string source;
      std::string timestamp;
   };
   NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(hr_data, bpm, source, timestamp)
}

namespace oura_charts::heart_rate
{
   using namespace detail;
   using std::move;
   using std::string;

   DataPoint::expected_value DataPoint::makeFromJson(const json& json_data) noexcept
   {
      try
      {
         hr_data data{};
         json_data.get_to(data);
         auto expected_timestamp = parseIsoDateTime(data.timestamp);
         if (expected_timestamp)
            return DataPoint{ data.bpm, expected_timestamp.value(), move(data.source) };
         else
            return unexpected{ expected_timestamp.error() };
      }
      catch (json::exception& e)
      {
         return unexpected{ translateException<DataPoint>(e) };
      }
      catch (std::exception& e)
      {
         return unexpected{ oura_exception{ fmt::format("unable to parse JSON for object: {}", e.what()), ErrorCategory::Parse }};
      }
   }

   DataPoint::DataPoint(int bpm, utc_timestamp timestamp, string source) noexcept
      : m_bpm(bpm), m_timestamp(timestamp), m_source(move(source))
   {
   }


} // namespace oura_charts::heart_rate
