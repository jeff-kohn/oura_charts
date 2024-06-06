//---------------------------------------------------------------------------------------------------------------------
// test_json_data.h
//
// unit tests for json serialization.h
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/json_structs.h"
#include "oura_charts/UserProfile.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/SleepSession.h"
#include "FileDataProvider.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

namespace oura_charts::test
{
   using namespace std::literals::chrono_literals;
   namespace fs = std::filesystem;


   TEST_CASE("test_parse_user_profile", "[parsing]")
   {
      // data provider for unit tests that gets json from disk files.
      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };

      auto exp_json = data_prov.getJsonObject(UserProfile::REST_PATH);
      REQUIRE(exp_json.has_value());

      auto exp_data = detail::readJson<detail::profile_data>(*exp_json);
      REQUIRE(exp_data.has_value());
   }


   TEST_CASE("test_parse_sleep_data", "[parsing]")
   {
      using namespace detail;

      // data provider for unit tests that gets json from disk files.
      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };

      auto exp_json = data_prov.getJsonObject(SleepSession::REST_PATH);
      REQUIRE(exp_json.has_value());

      auto exp_data = readJson<RestDataCollection<SleepSession::StorageType>>(*exp_json);
      REQUIRE(exp_data.has_value());

      auto sleep_data{ *exp_data };
      REQUIRE(sleep_data.data.size() > 1);
   }


   TEST_CASE("test_parse_heart_rate", "[parsing]")
   {
      using namespace detail;

      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };

      auto exp_json = data_prov.getJsonObject(HeartRate::REST_PATH);
      REQUIRE(exp_json.has_value());

      auto exp_data = readJson<RestDataCollection<HeartRate::StorageType>>(*exp_json);
      REQUIRE(exp_data.has_value());

      auto hr_data{ *exp_data };
      REQUIRE(hr_data.data.size() > 1);
   }


} // namespace oura_charts::test
