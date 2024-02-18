#include "oura_charts/HeartRate.h"

namespace oura_charts::detail
{
      struct hr_data
      {
         int bpm;
         std::string source;
         std::string timestamp;
      };

   NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(hr_data, bpm, source, timestamp)
}

namespace oura_charts
{
   using namespace detail;

   HeartRateDatum::HeartRateDatum(const json& json_data)
   {
      try
      {
         hr_data data{};
         json_data.get_to(data);



      }
      catch (json::exception& e)
      {
         throw translateException(e, this);
      }
   }
}
