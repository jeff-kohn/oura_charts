//---------------------------------------------------------------------------------------------------------------------
// get_sleep_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/DataSeries.h"
#include "helpers.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/SleepSession.h"
#include "oura_charts/TokenAuth.h"
#include "oura_charts/detail/logging.h"
#include <fmt/format.h>
#include <map>
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
      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      auto sleep_data = getDataSeries<SleepSession>(rest_server, today - years{ 1 }, today);


      // group the data by day of year. We'll move() the data from the DataSeries to our map to avoid copying.
      std::multimap<weekday, SleepSession> sleep_by_weekday{};
      
      //rg::for_each(sleep_data, [](SleepSession& session)
      //             {
      //                sleep_by_weekday.emplace


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
