#include "oura_charts/UserProfile.h"
#include "oura_charts/RestAuth.h"
#include "oura_charts/detail/utility.h"
#include <fmt/format.h>
#include <cxxopts.hpp>


// This example retrieves user-profile info from OURACHARTS_REST using Bearer authentication (PAT)
int main(int argc, char* argv[])
{
   try
   {
      using namespace oura_charts;
      using std::string;

      cxxopts::Options options(argv[0], "OuraCharts Get User Profile");
      options.add_options()
         ("t,token", "Personal Access Token for your Oura cloud account", cxxopts::value<string>()->default_value(""))
         ("h, help", "show help", cxxopts::value<bool>());

      // PAT token either needs to be supplied on the command line or in an environment variable.
      auto params = options.parse(argc, argv);
      string pat = params["token"].as<string>();
      if (pat.empty())
         pat = detail::getEnvironmentVariable("OURA_PAT");
      if (pat.empty())
         throw std::runtime_error("Personal Access Token not specified. Use the --token parameter or set the OURA_PAT environment variable.");

      if (params.count("help"))
      {
         fmt::println("{}", options.help());
         return 0;
      }

      auto profile = UserProfile::getProfile(AuthWrapper{ TokenAuth{pat} });
      fmt::println("Successfully retrieved {}", profile);

   }
   catch (std::exception& e)
   {
      fmt::println("Error: {}", e.what());
   }
}
