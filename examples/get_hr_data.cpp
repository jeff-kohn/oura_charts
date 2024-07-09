//---------------------------------------------------------------------------------------------------------------------
// get_hr_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/HeartRate.h"
#include "helpers.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/TokenAuth.h"
#include "oura_charts/detail/logging.h"
#include "oura_charts/functors.h"
#include <fmt/format.h>
#include <map>
#include <ranges>


// This example retrieves heart-rate data from the REST API
int main(int argc, char* argv[])
{
   using std::string;
   using fmt::println;
   using namespace oura_charts;
   namespace rg = std::ranges;

   auto logger = logging::LogFactory::makeDefault();
   try
   {
      cxxopts::Options options{ argv[0], "Get today's HR data from Oura Ring API." }; //NOLINT (cppcoreguidelines-pro-bounds-pointer-arithmetic)
      options.add_options()
         ("t,token", "Personal Access Token for your Oura cloud account", cxxopts::value<string>()->default_value(""))
         ("h,help", "show help", cxxopts::value<bool>());

      auto args = options.parse(argc, argv);
      if (args.count("help"))
      {
         println("{}", options.help());
         return 0;
      }
      auto pat{ getPersonalToken(args) };

      // Get all data from midnight local time to now.
      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      auto until = localNow();
      auto from = stripTimeOfDay(until);
      auto heart_rate_data = getDataSeries<HeartRate>(rest_server, from, until);

      // group HR measurements by hour of day.
      std::multimap<chrono::hours, int> heart_rates_by_hour{};
      for (auto& hr : heart_rate_data)
      {
         auto [ymd, tod] = getCivilTime(hr.timestamp());
         heart_rates_by_hour.insert(std::make_pair(chrono::floor<chrono::hours>(tod.hours()), hr.beatsPerMin()));
      }

      // Now for each hour, calculate the avergage of all the HR readings we have.
      auto it = heart_rates_by_hour.begin();
      while (it != heart_rates_by_hour.end())
      {
         // get the sub range representing all the values for the current key/hour and compute an average
         auto [beg, end] = heart_rates_by_hour.equal_range(it->first);
         std::ranges::subrange hour_range{ beg, end };

         AvgCalc<int> avg_calc{};
         rg::for_each(hour_range, std::ref(avg_calc), [] (auto&& val) { return val.second; });

         // need the hour range for display purposes
         auto start_time = from + it->first;
         auto end_time = start_time + 1h;

         // output the average to the console
         auto result = avg_calc.result();
         if (result.has_value())
            fmt::println("{:%I:%M%p}-{:%I:%M%p} average heart rate = {:.1f} bpm", start_time, end_time, result.value());

         // move to next hour range if we're not already at the last.
         it = end;
      }
   }
   catch (oura_exception& e)
   {
      logging::exception(e);
      println("Unable to retrieve data.\n\n{}", e);
   }
   catch (std::exception& e)
   {
      logging::exception(e);
      println("Unable to retrieve data.\n\n{}", e.what());
   }


}


