#include "oura_charts/RestAuth.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/detail/utility.h"
#include <fmt/format.h>
#include "helpers.h"

// This example retrieves heart-rate data from the REST API
int main(int argc, char* argv[])
{
   using std::string;
   using fmt::println;
   using namespace oura_charts;
   using namespace oura_charts::chrono;
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

      timestamp_local local_end = localTimestamp( localNow() );
      timestamp_local local_start = localTimestamp(chrono::floor<days>(local_end));

      auto hr_data = heart_rate::getDataSet(AuthWrapper{ TokenAuth{pat} }, local_start, local_end);

      println("Retrieved {} HR datapoints for today ({:%Om/%d/%Y}):", hr_data.size(), local_start);
      for (auto exp_hr : hr_data)
      {
         if (exp_hr)
            println("{}", *exp_hr);
         else
            println("[error {}", exp_hr.error());
      }
   }
   catch (oura_exception& e)
   {
      println("Unable to retrieve data.\n\n{}", e);
   }
   catch (std::exception& e)
   {
      println("Unable to retrieve data.\n\n{}", e.what());
   }


}

