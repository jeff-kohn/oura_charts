//---------------------------------------------------------------------------------------------------------------------
// get_sleep_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "helpers.h"
#include "oura_charts/chrono_helpers.h"
#include "oura_charts/detail/logging.h"
#include "oura_charts/concepts.h"
#include "oura_charts/DataSeries.h"
#include "oura_charts/DailySleepScore.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/SleepSession.h"
#include "oura_charts/TokenAuth.h"
#include "oura_charts/functors.h"
#include <fmt/format.h>
#include <tabulate/table.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <print>
#include <ranges>


// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)


auto getWeekdayTable()
{
   using tabulate::Table;
   using namespace oura_charts;

   auto weekday_names = getWeekdays() | vw::transform([] (weekday wd) -> std::string
                                                      {
                                                         return std::format("{:%A}", wd);
                                                      });
   Table weekdays{};
   Table::Row_t row{ "" }; // First column will have row headers 
   row.insert(row.end(), weekday_names.begin(), weekday_names.end());
   weekdays.add_row(row);
   return weekdays;
}

template <typename T>
auto format_value(std::optional<T> value, std::string_view default_val)
{
   using namespace oura_charts;

   if (value.has_value())
   {
      overload overloaded{[] (seconds secs) -> std::string
                         {
                            return std::format("{:%R}", hh_mm_ss{ secs });
                         },
                         [] (unsigned int val)
                         {
                            return std::format("{}", val);
                         },
                         [] (double val)
                         {
                            return std::format("{:.1f}", val);
                         }};
      return overloaded(value.value());
   }
   else
      return std::string{ default_val };
}


template<typename T>
auto createRow(std::string_view row_header, const std::vector<T>& col_data, std::string_view default_val = "")
{
   using tabulate::Table;
   using namespace oura_charts;

   auto col_text_values = vw::all(col_data) | vw::transform([default_val] (auto&& elem) -> std::string
                                                            {
                                                               return format_value(elem, default_val);
                                                            });

   Table::Row_t row{ row_header };
   row.insert(row.end(), col_text_values.begin(), col_text_values.end());
   return row;
}





// This example retrieves heart-rate data from the REST API
int main(int argc, char* argv[])
{
   using namespace oura_charts;
   using namespace oura_charts::chrono;
   using namespace std::chrono_literals;
   using namespace tabulate;
   using std::vector;
   using std::optional;

   auto logger = logging::LogFactory::makeDefault();

   try
   {
      cxxopts::Options options{ argv[0], "Get sleep session data for the given date range." }; //NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
      options.add_options()
         ("t,token", "Personal Access Token for your Oura cloud account", cxxopts::value<std::string>()->default_value(""))
         ("h,help", "show help", cxxopts::value<bool>());

      auto args = options.parse(argc, argv);
      if (args.count("help"))
      {
         fmt::println("{}", options.help());
         return 0;
      }
      auto pat{ getPersonalToken(args) };

      // Get sleep data for the past year. score is a separate data source
      auto today = stripTimeOfDay(localNow());
      auto last_week = today - months{ 12 };
      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      auto sleep_data = getDataSeries<SleepSession>(rest_server, getCalendarDate(last_week), getCalendarDate(today));
      auto score_data = getDataSeries<DailySleepScore>(rest_server, getCalendarDate(last_week), getCalendarDate(today));

      // group by day of week. in case of sleep we filter for only "long" sleep (no naps)
      auto sleep_by_weekday = group<SleepByWeekday>(std::move(sleep_data), sessionWeekday, long_sleep_filter);
      auto score_by_weekday = group<SleepScoreByWeekday>(std::move(score_data), sleepScoreWeekday);

      // calculate various averages by day of week.
      constexpr auto weekdays = getWeekdays();
      vector<nullable_double> avg_hrv(weekdays.size());
      vector<nullable_double> avg_resting_heart_rate(weekdays.size());
      vector<nullable_double> avg_score(weekdays.size());
      vector<optional<seconds>> avg_total_sleep(weekdays.size());

      for (auto wd : weekdays)
      {
         // get the sub range representing all the data for the current weekday. could be an empty range,
         // but that won't cause any problems we'll just get null result.
         auto [beg, end] = sleep_by_weekday.equal_range(wd);
         std::ranges::subrange sleep_range{ beg, end };

         // get the average for this weekday sub-range
         AvgCalc<double> calc_hrv{};
         AvgCalc<uint32_t> calc_resting_heart_rate{};
         AvgCalc<seconds> calc_total_sleep{};

         rg::for_each(sleep_range | vw::values, [&] (const SleepSession& session)
                      {
                         calc_hrv(session.avgHRV());
                         calc_resting_heart_rate(session.restingHeartRate());
                         calc_total_sleep(session.sleepTimeTotal());
                      });

         // now do same for sleep score.
         auto [score_beg, score_end] = score_by_weekday.equal_range(wd);
         std::ranges::subrange score_range{ score_beg, score_end };
         AvgCalc<uint32_t> calc_score{};
         rg::for_each(score_range | vw::values, [&] (const DailySleepScore& score)
                      {
                        calc_score(score.score());
                      });


         avg_hrv[wd.c_encoding()] = calc_hrv.result();  
         avg_resting_heart_rate[wd.c_encoding()] = calc_resting_heart_rate.result();
         avg_score[wd.c_encoding()] = calc_score.result();
         avg_total_sleep[wd.c_encoding()] = calc_total_sleep.result();
      }

      // Build our output table.
      auto result_table = getWeekdayTable();
      result_table.add_row(createRow("Avg Sleep Score", avg_score, "??"));
      result_table.add_row(createRow("Avg Total Sleep", avg_total_sleep, "??"));
      result_table.add_row(createRow("Avg HRV", avg_hrv, "??"));
      result_table.add_row(createRow("Resting Heart Rate", avg_resting_heart_rate, "??"));

      // display values right-justified
      result_table.format().font_align(FontAlign::right);

      result_table.row(1).format()
         .font_color(Color::yellow);

      result_table.row(3).format()
         .font_color(Color::green);

      result_table.row(4).format()
         .font_color(Color::red);

      // bold/color for row & column headers
      result_table[0].format()
         .font_align(FontAlign::center)
         .font_style({ FontStyle::bold })
         .font_color(Color::cyan);

      result_table.column(0).format()
         .font_align(FontAlign::left)
         .font_style({ FontStyle::bold });

      result_table.print(std::cout);
      std::cout << std::endl;
   }
   catch (oura_exception& e)
   {
      logging::exception(e);
      fmt::println("Unable to retrieve data.\n\n{}", e);
   }
   catch (std::exception& e)
   {
      logging::exception(e);
      fmt::println("Unable to retrieve data.\n\n{}", e.what());
   }

   return 0;
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
