//---------------------------------------------------------------------------------------------------------------------
// get_hr_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/HeartRateMeasurement.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/detail/utility.h"
#include "oura_charts/detail/logging.h"
#include <fmt/format.h>
#include "helpers.h"

// This example retrieves heart-rate data from the REST API
int main(int argc, char* argv[])
{
   using std::string;
   using fmt::println;
   using namespace oura_charts;

   auto logger = logging::LogFactory::makeDefault();
   try
   {

      cxxopts::Options options{ argv[0], "Get today's HR data from Oura Ring API." };
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
      auto hr_data = getDataSeries<HeartRateMeasurement>(rest_server, from, until);
      
      for (auto& hr : hr_data)
      {
         fmt::println("{}", hr);
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


