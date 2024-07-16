//---------------------------------------------------------------------------------------------------------------------
// test_SleepSession.cpp
//
// unit tests for SleepSession data object
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include "TestDataProvider.h"
#include "oura_charts/SleepSession.h"
#include "oura_charts/detail/json_structs.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace oura_charts::test
{
   using namespace constants;
   using namespace std::literals::chrono_literals;
   namespace fs = std::filesystem;



   TEST_CASE("test_parse_sleep_data", "[parsing]")
   {
      using namespace detail;

      // data provider for unit tests that gets json from disk files.
      auto data_prov = TestDataProvider{ fs::path{ UNIT_TEST_DATA_DIR } };

      auto json_res = data_prov.getJsonData(SleepSession::REST_PATH);
      REQUIRE(json_res.has_value());

      auto data_res = readJson<RestDataCollection<SleepSession::StorageType>>(*json_res);
      REQUIRE(data_res.has_value());

      auto sleep_data{ *data_res };
      REQUIRE(sleep_data.data.size() > 1);
   }


// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

} // namespace oura_charts::test
