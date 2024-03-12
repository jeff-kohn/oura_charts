//---------------------------------------------------------------------------------------------------------------------
// get_hr_data.cpp
//
// sample app that demonstrates retrieving heart rate information from the Oura Ring REST API
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/UserProfile.h"
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
   using namespace oura_charts::chrono;
   using namespace oura_charts::detail;
   using namespace oura_charts::constants;
   try
   {
      auto logger = logging::LogFactory::makeDefault();

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

      timestamp_local local_end = localTimestamp( localNow() );
      timestamp_local local_start = localTimestamp(floor<days>(local_end - days{ 7 }));

      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      cpr::Parameters params{
         { REST_PARAM_START_DATE, toIsoDate(localToUtc(local_start)) },
         { REST_PARAM_END_DATE, toIsoDate(localToUtc(local_end))}
      };

      auto exp_json = rest_server.getJsonObject("sleep", params);
      if (exp_json)
         saveTextToFile(glz::prettify(exp_json.value()), "sleep.json");
      else
         throw exp_json.error();


      //auto expect_hr = rest_server.getJsonDataSeries<hr_data>(constants::REST_PATH_HEART_RATE, localToUtc(local_start), localToUtc(local_end));
      //if (!expect_hr)
      //   throw expect_hr.error();

      //for (auto& hr : expect_hr->data)
      //{
      //   fmt::println("{}bpm at {} ({})", hr.bpm, hr.timestamp, hr.source);
      //}
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


