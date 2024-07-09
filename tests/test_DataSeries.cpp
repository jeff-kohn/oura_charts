#include "TestDataProvider.h"
#include "oura_charts/DataSeries.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/chrono_helpers.h"
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <set>

namespace oura_charts::test
{
   using namespace std::literals;
   using namespace detail;

   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

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


      // generate range containing the specified bpm values for the first 28 days
      // of each month.
   auto generateHeartRateSeries(rg::input_range auto&& bpm_values, uint32_t num_days = 7)
   {
      const auto today = getCalendarDate(localNow());
      const auto this_year = today.year();

      // generate some test data. 
      std::vector<hr_data> hr_structs{};
      auto src = "test"s;
      auto months = getMonths();
      for (month m : months)
      {
         for (unsigned int d = 1; d <= num_days; ++d)
         {
            auto ymd = this_year / m / d;
            for (bpm : bpm_values())
            {
               hr_structs.emplace_back(hr_data{ bpm, src, local_days{ymd} });
            }
         }
      }
      return HeartRateDataSeries{ hr_structs };
   }


   TEST_CASE("test_HeartRateSeries_group_by_month")
   {
      // generate some data. 2 measurements per day, 28 days per month.
      const int num_days = 28;
      auto hr_series = generateHeartRateSeries(std::array{ 60, 70 }, num_days);

      // group by month
      MapByMonth<HeartRate> hr_by_month{};
      groupBy(std::move(hr_series), hr_by_month, HeartRateMonth);

      // make sure we got a subrange for each month
      constexpr auto months = getMonths();
      std::set<month> key_list{ vw::keys(hr_by_month) | rg::to<std::set<month>>() };
      REQUIRE( ssize(months) == ssize(key_list) );

      // make sure we got the expected number of entries with the expected min/max/average
      for (const auto& month : months)
      {
         auto [beg, end] = hr_by_month.equal_range(month);
         rg::subrange hr_month{ beg, end };

         REQUIRE(rg::distance(hr_month) == 2 * num_days); // we created 2x/per day

         // get the average for this month's sub-range
         AvgCalc<int> avg_calc{};
         MinCalc<int> min_calc{};
         MaxCalc<int> max_calc{};
         rg::for_each( hr_month | vw::values, std::ref(avg_calc), &HeartRate::beatsPerMin );
         rg::for_each( hr_month | vw::values, std::ref(min_calc), &HeartRate::beatsPerMin );
         rg::for_each( hr_month | vw::values, std::ref(max_calc), &HeartRate::beatsPerMin );
         REQUIRE(avg_calc.hasResult());
         REQUIRE(min_calc.hasResult());
         REQUIRE(max_calc.hasResult());
         REQUIRE(avg_calc.result().value() == 65);
         REQUIRE(min_calc.result().value() == 60);
         REQUIRE(max_calc.result().value() == 70);
      }
   }

   TEST_CASE("test_HeartRateSeries_group_by_weekday")
   {
      // generate some data. 2 measurements per day, 28 days per month.
      const int num_days = 7;
      auto hr_series = generateHeartRateSeries(std::array{ 60, 70 }, num_days);

      // group by weekday
      MapByWeekday<HeartRate> hr_by_weekday{};
      groupBy(std::move(hr_series), hr_by_weekday, HeartRateWeekday);

      // make sure we got a subrange for each month
      constexpr auto weekdays = getWeekdays();
      using WeekdaySet = std::set<weekday, weekday_compare_less>;
      WeekdaySet key_list{ hr_by_weekday | vw::keys  | rg::to<WeekdaySet>()};

      REQUIRE(ssize(weekdays) == ssize(key_list));

      // make sure we got the expected number of entries with the expected min/max/average
      for (const auto& wd : weekdays)
      {
         auto [beg, end] = hr_by_weekday.equal_range(wd);
         rg::subrange hr_weekday{ beg, end };

         REQUIRE(rg::distance(hr_weekday) == 24); // 2 data points per weekday per month.

         // get the average for this month's sub-range
         AvgCalc<int> avg_calc{};
         MinCalc<int> min_calc{};
         MaxCalc<int> max_calc{};
         rg::for_each(hr_weekday | vw::values, std::ref(avg_calc), &HeartRate::beatsPerMin);
         rg::for_each(hr_weekday | vw::values, std::ref(min_calc), &HeartRate::beatsPerMin);
         rg::for_each(hr_weekday | vw::values, std::ref(max_calc), &HeartRate::beatsPerMin);
         REQUIRE(avg_calc.hasResult());
         REQUIRE(min_calc.hasResult());
         REQUIRE(max_calc.hasResult());
         REQUIRE(avg_calc.result().value() == 65);
         REQUIRE(min_calc.result().value() == 60);
         REQUIRE(max_calc.result().value() == 70);
      }
   }

   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

} // namespace oura_charts::test
