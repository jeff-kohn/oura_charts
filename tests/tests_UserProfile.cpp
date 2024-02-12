#include "oura_charts/UserProfile.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   static constexpr const char *ID = "jk";
   static constexpr const char *BIOLOGICAL_SEX = "male";
   static constexpr const char *EMAIL = "jk@mail.com";
   static constexpr int AGE = 33;
   static constexpr int WEIGHT_KG = 33;
   static constexpr int HEIGHT_CM = 33;

   TEST_CASE("Structured bindings for UserProfile", "UserProfile")
   {
      using namespace oura_charts::detail;

      user_data user{ ID, EMAIL, AGE, WEIGHT_KG, HEIGHT_CM, BIOLOGICAL_SEX };

      UserProfile profile{ std::move(user) };
      auto& [id, email, age, weight, height, sex ] = profile;

      REQUIRE(id == ID);
      REQUIRE(age == AGE);
      REQUIRE(weight == WEIGHT_KG);
      REQUIRE(height == HEIGHT_CM);
      REQUIRE(sex == BIOLOGICAL_SEX);
      REQUIRE(email == EMAIL);
   }
}
