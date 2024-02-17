#include "oura_charts/HeartRate.h"

namespace oura_charts::detail
{
   NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(hr_data, bpm, source, timestamp)
}

namespace oura_charts
{
   using namespace detail;

   HeartRateDatum::HeartRateDatum(const json& json_data)
   {
      try
      {
         json_data.get_to(static_cast<hr_data&>(*this));
      }
      catch (json::exception& e)
      {
         throw translateException(e, this);
      }
   }
}
