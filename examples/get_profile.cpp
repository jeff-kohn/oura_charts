#include "oura_charts/UserProfile.h"
#include "oura_charts/RestAuth.h"
#include "oura_charts/helpers.h"
#include <fmt/format.h>
#include <cxxopts.hpp>



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

      auto params = options.parse(argc, argv);
      string pat = params["token"].as<string>();
      if (pat.empty())
         pat = helpers::getEnvironmentVariable("OURA_PAT");
      if (pat.empty())
         throw std::runtime_error("Personal Access Token not specified. Use the --token parameter or set the OURA_TOKEN environment variable.");

      if (params.count("help"))
      {
         fmt::println("{}", options.help());
         return 0;
      }

      // Create an instance of the rest client
      auto rest_client = restc_cpp::RestClient::Create();
      oura_charts::TokenAuth auth{ pat };
      auto profile = oura_charts::UserProfile::getProfile(auth);

   }
   catch (std::exception& e)
   {
      fmt::println("Error: {}", e.what());
   }
}
