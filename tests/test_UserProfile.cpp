//---------------------------------------------------------------------------------------------------------------------
// test_userProfile.h
//
// unit tests for the UserProfile class
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "oura_charts/oura_charts.h"
#include "oura_charts/RestDataProvider.h"
#include "oura_charts/UserProfile.h"
#include "oura_charts/detail/utility.h"
#include "oura_charts/detail/logging.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   log::LogInit logger{ log::TestLogger::make() };

   //auto json_profile = R"(
   //   {
   //      "id":"123",
   //      "age":21,
   //      "weight":85.3,
   //      "height":1.65,
   //      "biological_sex":"male",
   //      "email":"user@email.com"
   //   }
   //)"_json;

   TEST_CASE("Online UserProfile Test", "[online][rest]")
   {
      auto pat = detail::getEnvironmentVariable("OURA_PAT");
      if (pat.empty())
         SKIP("No PAT environment variable found for authenticating online tests");

      log::info("PAT environment variable found, running online tests");

      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      REQUIRE_NOTHROW(UserProfile::getUserProfile(rest_server));
   }

   //TEST_CASE("UserProfile Parse and Bindings", "[json][UserProfile]")
   //{
   //   using std::string;

   //   UserProfile p{ json_profile };

   //   REQUIRE(json_profile["id"].get<string>() == p.id());
   //   REQUIRE(json_profile["age"].get<int>() == p.age());
   //   REQUIRE(json_profile["weight"].get<double>() == p.weight());
   //   REQUIRE(json_profile["height"].get<double>() == p.height());
   //   REQUIRE(json_profile["biological_sex"].get<string>() == p.biologicalSex());
   //   REQUIRE(json_profile["email"].get<string>() == p.email());


   //   auto& [id, email, age, weight, height, sex ] = p;

   //   REQUIRE(id == p.id());
   //   REQUIRE(age == p.age());
   //   REQUIRE(weight == p.weight());
   //   REQUIRE(height == p.height());
   //   REQUIRE(sex == p.biologicalSex());
   //   REQUIRE(email == p.email());
   //}
}
