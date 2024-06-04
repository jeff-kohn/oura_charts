//---------------------------------------------------------------------------------------------------------------------
// test_json_data.h
//
// unit tests for json serialization.h
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/json_structs.h"
#include "FileDataProvider.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

namespace oura_charts::test
{
   using namespace std::literals::chrono_literals;
   namespace fs = std::filesystem;


   using namespace std::literals;

 
   auto user_profile_json = R"(
      {
         "id":"123",
         "age":21,
         "weight":85.3,
         "height":1.65,
         "biological_sex":"male",
         "email":"user@email.com"
      }
   )"sv;

   TEST_CASE("parse_user_profile", "[user_profile][parsing]")
   {
      using namespace detail;
      auto exp_data = readJson<profile_data>(user_profile_json);

      REQUIRE(exp_data.has_value());

      auto&& user_data{ *exp_data };
      REQUIRE(user_data.id == "123");
      REQUIRE(user_data.age == 21);
      REQUIRE(user_data.weight == 85.3);
      REQUIRE(user_data.height == 1.65);
      REQUIRE(user_data.biological_sex == "male");
      REQUIRE(user_data.email == "user@email.com");
   }


   TEST_CASE("parse_sleep", "[sleep][parsing]")
   {
      using namespace detail;

      // Load the json data.
      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };
      auto exp_json = data_prov.getJsonObject("sleep");
      REQUIRE(exp_json.has_value());

      auto exp_data = readJson<sleep_data_series>(*exp_json);
      REQUIRE(exp_data.has_value());

      auto&& sleep_data{ *exp_data };
      REQUIRE(sleep_data.data.size() > 0);
   }

   TEST_CASE("parse_heart_rate", "[heart_rate][parsing]")
   {

   }


} // namespace oura_charts::test
