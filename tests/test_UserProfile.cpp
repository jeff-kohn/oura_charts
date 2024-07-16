//---------------------------------------------------------------------------------------------------------------------
// test_UserProfile.cpp
//
// unit tests for the UserProfile data object
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "TestDataProvider.h"
#include "oura_charts/UserProfile.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

   using namespace constants;

   constexpr inline auto json = R"({
         "id": "1",
         "age" : 10,
         "weight" : 1.1,
         "height" : 1.1,
         "biological_sex" : "male",
         "email" : "user@email.com"
   })";

   TEST_CASE("test_parse_user_profile", "[parsing]")
   {
      auto data_res = detail::readJson<detail::profile_data>(json);
      REQUIRE(data_res.has_value());
      REQUIRE(data_res->id == "1");
      REQUIRE(data_res->weight == 1.1);
      REQUIRE(data_res->height == 1.1);
      REQUIRE(data_res->biological_sex == "male");
      REQUIRE(data_res->email == "user@email.com");
   }


   TEST_CASE("test_UserProfile_bindings", "[binding]")
   {
      // data provider for unit tests that gets json from disk files.
      auto data_prov = TestDataProvider{ fs::path{ UNIT_TEST_DATA_DIR } };

      UserProfile profile{ getUserProfile(data_prov) };
      auto& [id, email, age, weight, height, sex] = profile;

      REQUIRE(id == profile.id());
      REQUIRE(age == profile.age());
      REQUIRE(weight == profile.weight());
      REQUIRE(height == profile.height());
      REQUIRE(sex == profile.biologicalSex());
      REQUIRE(email == profile.email());
   }

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

} // namespace oura_charts::test
