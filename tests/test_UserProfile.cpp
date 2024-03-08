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
   using namespace std::literals;
   
   auto default_logger = logging::LogFactory::makePrivate("UnitTesting",
                                                          { logging::makeConsoleSink(logging::level_enum::info), logging::makeDebuggerSync() },
                                                          logging::level_enum::trace );

   auto json_profile = R"(
      {
         "id":"123",
         "age":21,
         "weight":85.3,
         "height":1.65,
         "biological_sex":"male",
         "email":"user@email.com"
      }
   )"sv;

   TEST_CASE("Online UserProfile Test", "[online][rest][UserProfile]")
   {
      auto pat = detail::getEnvironmentVariable("OURA_PAT");
      if (pat.empty())
         SKIP("No PAT environment variable found for authenticating online tests");

      logging::info("PAT environment variable found, running online tests");

      RestDataProvider rest_server{ TokenAuth{pat}, constants::REST_DEFAULT_BASE_URL };
      REQUIRE_NOTHROW(UserProfile::getUserProfile(rest_server));
   }

   TEST_CASE("UserProfile Parse and Bindings", "[json][UserProfile]")
   {
      using std::string;

      REQUIRE_NOTHROW(UserProfile{ json_profile });

      UserProfile p{ json_profile };
      auto& [id, email, age, weight, height, sex ] = p;

      REQUIRE(id == p.id());
      REQUIRE(age == p.age());
      REQUIRE(weight == p.weight());
      REQUIRE(height == p.height());
      REQUIRE(sex == p.biologicalSex());
      REQUIRE(email == p.email());
   }
}
