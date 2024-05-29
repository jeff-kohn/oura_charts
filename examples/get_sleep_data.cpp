//---------------------------------------------------------------------------------------------------------------------
// get_sleep_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/UserProfile.h"
#include "oura_charts/DataSeries.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/SleepSession.h"
#include "oura_charts/detail/logging.h"
#include <fmt/format.h>
#include "helpers.h"

// This example retrieves heart-rate data from the REST API
int main(int argc, char* argv[])
{
   using namespace oura_charts;
   using namespace oura_charts::chrono;

   auto logger = logging::LogFactory::makeDefault();

   try
   {
      cxxopts::Options options{ argv[0], "Get sleep session data for the given date range." };
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

      auto end = localTimestamp(localNow());
      auto begin = localTimestamp(floor<days>(end - days{ 7 }));
      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      auto sleep_data = getDataSeries<SleepSession>(rest_server, begin, end);

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
