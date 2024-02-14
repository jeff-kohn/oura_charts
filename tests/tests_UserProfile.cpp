#include "oura_charts/UserProfile.h"
#include "oura_charts/helpers.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{

   TEST_CASE("Online UserProfile Test", "Online")
   {
      auto pat = helpers::getEnvironmentVariable("OURA_PAT");
      if (pat.empty())
         SKIP("No PAT environment variable found for authenticating online tests");

      REQUIRE_NOTHROW(UserProfile::getProfile(AuthWrapper{ TokenAuth{pat} }));
   }

   TEST_CASE("Structured bindings for UserProfile", "UserProfile")
   {

      //UserProfile profile{  };
      //auto& [id, email, age, weight, height, sex ] = profile;

      //REQUIRE(id == ID);
      //REQUIRE(age == AGE);
      //REQUIRE(weight == WEIGHT_KG);
      //REQUIRE(height == HEIGHT_CM);
      //REQUIRE(sex == BIOLOGICAL_SEX);
      //REQUIRE(email == EMAIL);
   }
}
