//---------------------------------------------------------------------------------------------------------------------
// get_hr_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/HeartRate.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/functors.h"
#include "oura_charts/TokenAuth.h"
#include "oura_charts/detail/logging.h"
#include "helpers.h"
#include <fmt/format.h>
#include <map>

// This example retrieves heart-rate data from the REST API
int main(int argc, char* argv[])
{
   using std::string;
   using fmt::println;
   using namespace oura_charts;

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

      auto it = heart_rates_by_hour.cbegin();
      while (it != heart_rates_by_hour.cend())
      {
         // get the sub range representing all the values for the current key/hour and compute an average
         auto hour_range = heart_rates_by_hour.equal_range(it->first);
         auto acc = std::accumulate(hour_range.first, hour_range.second, AverageAccumulator<int>{},
                                    [] (auto&& avg, auto kvp)
                                    {
                                       return avg += kvp.second;
                                    });

         // output the average to the console, need the hour range for labeling
         auto start_time = from + it->first;
         auto end_time = start_time + 1h;

         fmt::println("{:%I:%M%p}-{:%I:%M%p} average heart rate = {:.1f} bpm", start_time, end_time, acc.getAverage());

         // move to next hour range if we're not already at the last.
         it = hour_range.second;
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


