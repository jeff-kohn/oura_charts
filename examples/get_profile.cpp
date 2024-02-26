#include "oura_charts/UserProfile.h"
#include "oura_charts/RestAuth.h"
#include "oura_charts/detail/utility.h"
#include <fmt/format.h>
#include "helpers.h"

// This example retrieves user-profile info from OURACHARTS_REST using Bearer authentication (PAT)
int main(int argc, char* argv[])
{
   using namespace oura_charts;

   try
   {
      using std::string;

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
      auto profile = UserProfile::getProfile(AuthWrapper{ TokenAuth{pat} });
      fmt::println("Successfully retrieved {}", profile);
   }
   catch (oura_exception& e)
   {
      fmt::println("Unable to retrieve profile.\n\n{}", e);
   }
   catch (std::exception& e)
   {
      fmt::println("Unable to retrieve profile.\n\n{}", e.what());
   }
}
