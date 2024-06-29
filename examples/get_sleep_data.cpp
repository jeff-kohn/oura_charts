//---------------------------------------------------------------------------------------------------------------------
// get_sleep_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "helpers.h"
#include "oura_charts/DataSeries.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/SleepSession.h"
#include "oura_charts/TokenAuth.h"
#include "oura_charts/datetime_helpers.h"
#include "oura_charts/detail/logging.h"
#include "oura_charts/functors.h"
#include <algorithm>
#include <chrono>
#include <fmt/format.h>
#include <map>
#include <print>
#include <ranges>


// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

// This example retrieves heart-rate data from the REST API
int main(int argc, char* argv[])
{
   using namespace oura_charts;
   using namespace oura_charts::chrono;
   using namespace std::chrono_literals;

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

      // Get sleep date for the past year.
      auto today = stripTimeOfDay(localNow());
      auto last_week = today - months{ 12 };
      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      auto sleep_data = getDataSeries<SleepSession>(rest_server, getCalendarDate(last_week), getCalendarDate(today));

      // create a filter to only get "long" sleep sessions (don't include naps/rest, they throw off the averages).
      auto filt = sleep_data | vw::filter(SleepTypeFilter{ SleepSession::SleepType::long_sleep });

      // Now sort the filtered view into buckets by day of week.
      using SleepByWeekdayMap = std::multimap<weekday, SleepSession, weekday_compare_less>;
      SleepByWeekdayMap sleep_by_weekday{};
      rg::for_each(filt, [&sleep_by_weekday](SleepSession& session)
                  {
                     sleep_by_weekday.emplace(weekday{session.sessionDate()}, std::move(session));
                  });

      // calculate the average of all the sleep sessions for each weekday
      for (auto wd : getWeekdays())
      {
         // get the sub range representing all the data for the current weekday.
         auto [beg, end] = sleep_by_weekday.equal_range(wd);
         if (beg == end)
            continue;

         std::ranges::subrange sleep_range{ beg, end };

         // get the average for this weekday sub-range
         AvgCalc<seconds> avg_sleep_time{};
         rg::for_each(sleep_range | vw::values, std::ref(avg_sleep_time), &SleepSession::sleepTimeTotal);

         // note we decrement the weekday, because the date is actually the following moring.
         std::println("{} ({}): {} sleep on average", --wd, avg_sleep_time.count(), hh_mm_ss{avg_sleep_time.result()});
      }

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
