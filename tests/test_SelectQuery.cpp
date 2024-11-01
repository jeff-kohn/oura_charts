//---------------------------------------------------------------------------------------------------------------------
// test_SelectQuery.cpp
//
// unit tests for SelectQuery object
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------
#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/json_structs.h"
#include "oura_charts/HeartRateTraits.h"
#include "TestDataProvider.h"

#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace oura_charts::test
{
   using namespace constants;
   using namespace std::literals::chrono_literals;


   TEST_CASE("test_simple_select_query", "[query]")
   {
      // data provider for unit tests that gets json from disk files.
      auto data_prov = TestDataProvider{ fs::path{ UNIT_TEST_DATA_DIR } };

      HeartRateSeries hr_data{ detail::getDataSeries<HeartRate>(data_prov, detail::SortedPropertyMap{}) };

   }

}  // namespace oura_charts::test
