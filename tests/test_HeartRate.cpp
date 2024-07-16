//---------------------------------------------------------------------------------------------------------------------
// test_HeartRate.cpp
//
// unit tests for HeartRate data object
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include "TestDataProvider.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/detail/json_structs.h"
#include <catch2/catch_test_macros.hpp>
#include <set>

namespace oura_charts::test
{
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access)

   using namespace constants;
   using namespace detail;
   using namespace std::literals;


   TEST_CASE("test_parse_heart_rate", "[parsing]")
   {

      auto data_prov = TestDataProvider{ fs::path{ UNIT_TEST_DATA_DIR } };

      auto json_res = data_prov.getJsonData(HeartRate::REST_PATH);
      REQUIRE(json_res.has_value());

      auto data_res = readJson<RestDataCollection<HeartRate::StorageType>>(*json_res);
      REQUIRE(data_res.has_value());

      auto hr_data{ *data_res };
      REQUIRE(hr_data.data.size() > 1);
   }


   TEST_CASE("test_HeartRate_bindings", "[binding]")
   {
      // data provider for unit tests that gets json from disk files.
      auto data_prov = TestDataProvider{ fs::path{ UNIT_TEST_DATA_DIR } };

      HeartRateSeries hr_data{ detail::getDataSeries<HeartRate>(data_prov, detail::SortedPropertyMap{}) };

      REQUIRE(hr_data.size() > 0);

      auto&& hr = hr_data[0];
      auto& [bpm, source, timestamp] = hr;

      REQUIRE(bpm == hr.beatsPerMin());
      REQUIRE(source == hr.source());
      REQUIRE(timestamp == hr.timestamp());
   }
  

   TEST_CASE("test_HeartRateSeries_paging", "[parsing][binding]")
   {
      TestDataProvider provider{ constants::UNIT_TEST_DATA_DIR };

      // retreive the default test data for HR
      HeartRateSeries non_paged{ detail::getDataSeries<HeartRate>(provider, detail::SortedPropertyMap{}) };

      // paginate the data, tripling it to 3 pages and retrieve again
      REQUIRE_NOTHROW(provider.paginateDataSource(constants::REST_PATH_HEART_RATE, 3));

      HeartRateSeries paged{ detail::getDataSeries<HeartRate>(provider, detail::SortedPropertyMap{}) };
      REQUIRE(paged.size() == non_paged.size() * 3);
   }


   // generate range containing the specified bpm values for the first 28 days
   // of each month.
   auto generateHeartRateSeries(rg::input_range auto&& bpm_values, int num_days = 7)
   {
      const auto today = getCalendarDate(localNow());
      const auto this_year = today.year();

      // generate some test data. 
      std::vector<hr_data> hr_structs{};
      auto src = "test"s;
      auto months = getMonths();
      for (month m : months)
      {
         for (auto d = 1; d <= num_days; ++d)
         {
            auto ymd = this_year / m / d;
            for (auto bpm : bpm_values)
            {
               hr_structs.emplace_back(hr_data{ bpm, src, local_days{ymd} });
            }
         }
      }
      return HeartRateSeries{ hr_structs };
   }


   TEST_CASE("test_HeartRateSeries_group_by_month")
   {
      // generate some data. 2 measurements per day, 28 days per month.
      const int num_days = 28;
      auto hr_series = generateHeartRateSeries(std::array{ 60, 70 }, num_days);

      // group by month
      MapByMonth<HeartRate> hr_by_month{};
      groupBy(std::move(hr_series), hr_by_month, heartRateMonth);

      // make sure we got a subrange for each month
      constexpr auto months = getMonths();
      std::set<month> key_list{ vw::keys(hr_by_month) | rg::to<std::set<month>>() };
      REQUIRE(ssize(months) == ssize(key_list));

      // make sure we got the expected number of entries with the expected min/max/average
      for (const auto& month : months)
      {
         auto [beg, end] = hr_by_month.equal_range(month);
         rg::subrange hr_month{ beg, end };

         REQUIRE(static_cast<int>(rg::distance(hr_month)) == 2 * num_days); // we created 2x/per day

         // get the average for this month's sub-range
         AvgCalc<int> avg_calc{};
         MinCalc<int> min_calc{};
         MaxCalc<int> max_calc{};
         rg::for_each(hr_month | vw::values,
                      [&] (const auto& hr)
                      {
                         avg_calc(hr);
                         min_calc(hr);
                         max_calc(hr);
                      },
                      &HeartRate::beatsPerMin);

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
      groupBy(std::move(hr_series), hr_by_weekday, heartRateWeekday);

      // make sure we got a subrange for each month
      constexpr auto weekdays = getWeekdays();
      using WeekdaySet = std::set<weekday, weekday_compare_less>;
      WeekdaySet key_list{ hr_by_weekday | vw::keys | rg::to<WeekdaySet>() };

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

         rg::for_each(hr_weekday | vw::values,
                      [&] (const auto& hr)
                      {
                         avg_calc(hr);
                         min_calc(hr);
                         max_calc(hr);
                      },
                      &HeartRate::beatsPerMin);

         REQUIRE(avg_calc.hasResult());
         REQUIRE(min_calc.hasResult());
         REQUIRE(max_calc.hasResult());
         REQUIRE(avg_calc.result().value() == 65);
         REQUIRE(min_calc.result().value() == 60);
         REQUIRE(max_calc.result().value() == 70);
      }
   }

   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access)


}// namespace oura_charts::test
