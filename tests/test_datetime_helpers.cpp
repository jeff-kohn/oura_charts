
#include "oura_charts/oura_charts.h"
//#include "oura_charts/detail/rest_helpers.h"
#include "oura_charts/datetime_helpers.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   using namespace std::literals::chrono_literals;

   TEST_CASE("Parse_IsoDateString", "[datetime][parsing]")
   {
      chrono::year_month_day ymd{ 2024y / 2 / 1 };
      chrono::hh_mm_ss tod{ 12h + 3min + 33s };
      timestamp_utc tp = chrono::sys_days{ ymd } + tod.to_duration();

      std::string date_str{ "2024-02-01T12:03:33Z" };
      std::string local_date{ "2024-02-01T07:03:33-5:00" };

      REQUIRE(tp == parseIsoDateTime(local_date));
      REQUIRE(tp == parseIsoDateTime(date_str));
      REQUIRE(date_str == toIsoDateString(tp));
   }


} // namespace oura_charts::test
