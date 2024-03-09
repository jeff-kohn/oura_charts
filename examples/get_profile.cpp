//---------------------------------------------------------------------------------------------------------------------
// get_profile.cpp
//
// sample app that demonstrates retrieving a user profile from the Oura Ring REST API.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/UserProfile.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/detail/utility.h"
#include "oura_charts/detail/logging.h"
#include <fmt/format.h>
#include "helpers.h"

// This example retrieves user-profile info from OURACHARTS_REST using Bearer authentication (PAT)
int main(int argc, char* argv[])
{
   using namespace oura_charts;

   try
   {
      using std::string;

      auto logger = logging::LogFactory::makeDefault();

      cxxopts::Options options(argv[0], "OuraCharts Get User Profile");
      options.add_options()
         ("t,token", "Personal Access Token for your Oura cloud account", cxxopts::value<string>()->default_value(""))
         ("h,help", "show help", cxxopts::value<bool>());

      auto args = options.parse(argc, argv);
      if (args.count("help"))
      {
         fmt::println("{}", options.help());
         return 0;
      }
      auto pat = getPersonalToken(args);

      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL, logger };
      auto profile = UserProfile::getUserProfile(rest_server);
      fmt::println("Successfully retrieved {}", profile);
   }
   catch (oura_exception& e)
   {
      logging::exception(e);
      fmt::println("Unable to retrieve profile.\n\n{}", e);
   }
   catch (std::exception& e)
   {
      logging::exception(e);
      fmt::println("Unable to retrieve profile.\n\n{}", e.what());
   }
}
