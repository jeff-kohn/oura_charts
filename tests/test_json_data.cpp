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


   TEST_CASE("parse_sleep", "[sleep][parsing]")
   {
      // Load the json data.
      auto data_prov = FileDataProvider{ fs::path{"./test_data" } };
      auto exp_json = data_prov.getJsonObject("sleep");

      REQUIRE(exp_json.has_value());

      // parse into structs
      detail::sleep_data_series sleep_data{};
      auto pe = glz::read<glz::opts{ .error_on_unknown_keys = false }>(sleep_data, exp_json.value());
      if (pe)
         throw oura_exception{
                  static_cast<int64_t>(pe.ec),
                  glz::format_error(pe, exp_json.value()),
                  ErrorCategory::Parse };

   }


} // namespace oura_charts::test
