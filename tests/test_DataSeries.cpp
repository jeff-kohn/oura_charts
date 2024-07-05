#include "TestDataProvider.h"
#include "oura_charts/DataSeries.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/SleepSession.h"
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

namespace oura_charts::test
{
   using namespace std::string_view_literals;
   using namespace detail;

   TEST_CASE("test_DataSeries_paging", "[parsing][binding]")
   {
      TestDataProvider provider{ constants::UNIT_TEST_DATA_DIR };

      // retreive the default test data for HR
      HeartRateDataSeries non_paged{ detail::getDataSeries<HeartRate>(provider, detail::SortedPropertyMap{}) };

      // paginate the data, tripling it to 3 pages and retrieve again
      REQUIRE_NOTHROW(provider.paginateDataSource(constants::REST_PATH_HEART_RATE, 3));

      HeartRateDataSeries paged{ detail::getDataSeries<HeartRate>(provider, detail::SortedPropertyMap{}) };
      REQUIRE(paged.size() == non_paged.size() * 3);
   }


   TEST_CASE("test_SleepSessionSeries_grouping")
   {
      TestDataProvider provider{ constants::UNIT_TEST_DATA_DIR };

      auto sleep_sessions = getDataSeries<SleepSession>(provider, detail::SortedPropertyMap{});

      //auto by_weekday = group<SleepByWeekday>(SleepSessionSeries{ sleep_sessions }, SessionWeekday);
//      auto by_year_month = groupByYearMonth(SleepSessionSeries{ sleep_sessions }, SleepTypeFilter{});
   }
} // namespace oura_charts::test
