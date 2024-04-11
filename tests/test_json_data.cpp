//---------------------------------------------------------------------------------------------------------------------
// test_json_data.h
//
// unit tests for json serialization.h
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include "FileDataProvider.h"
#include <catch2/catch_test_macros.hpp>
#include <filesystem>

namespace oura_charts::test
{
   using namespace std::literals::chrono_literals;
   namespace fs = std::filesystem;

   TEST_CASE("parse_sleep", "[sleep][parsing]")
   {
      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };
      auto exp_json = data_prov.getJsonObject("sleep");

      REQUIRE(exp_json.has_value());
   }


} // namespace oura_charts::test
