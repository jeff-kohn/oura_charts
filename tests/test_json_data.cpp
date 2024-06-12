//---------------------------------------------------------------------------------------------------------------------
// test_json_data.h
//
// unit tests for json serialization.h
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include "FileDataProvider.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/SleepSession.h"
#include "oura_charts/UserProfile.h"
#include "oura_charts/detail/json_structs.h"
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

      auto json_res = data_prov.getJsonObject(UserProfile::REST_PATH);
      REQUIRE(json_res.has_value());

      auto data_res = detail::readJson<detail::profile_data>(*json_res);
      REQUIRE(data_res.has_value());
   }


   TEST_CASE("test_parse_sleep_data", "[parsing]")
   {
      using namespace detail;

      // data provider for unit tests that gets json from disk files.
      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };

      auto json_res = data_prov.getJsonObject(SleepSession::REST_PATH);
      REQUIRE(json_res.has_value());

      auto data_res = readJson<RestDataCollection<SleepSession::StorageType>>(*json_res);
      REQUIRE(data_res.has_value());

      auto sleep_data{ *data_res };
      REQUIRE(sleep_data.data.size() > 1);
   }


   TEST_CASE("test_parse_heart_rate", "[parsing]")
   {
      using namespace detail;

      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };

      auto json_res = data_prov.getJsonObject(HeartRate::REST_PATH);
      REQUIRE(json_res.has_value());

      auto data_res = readJson<RestDataCollection<HeartRate::StorageType>>(*json_res);
      REQUIRE(data_res.has_value());

      auto hr_data{ *data_res };
      REQUIRE(hr_data.data.size() > 1);
   }


} // namespace oura_charts::test
