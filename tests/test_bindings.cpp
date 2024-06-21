//---------------------------------------------------------------------------------------------------------------------
// test_bindings.h
//
// unit tests for the binding interfaces of classes that support it
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "TestDataProvider.h"
#include "oura_charts/HeartRate.h"
#include "oura_charts/UserProfile.h"
#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{

   TEST_CASE("test_UserProfile_bindings", "[binding]")
   {
      // data provider for unit tests that gets json from disk files.
      auto data_prov = TestDataProvider{ fs::path{"./test_data" } };

      UserProfile profile{ getUserProfile(data_prov) };
      auto& [id, email, age, weight, height, sex] = profile;

      REQUIRE(id == profile.id());
      REQUIRE(age == profile.age());
      REQUIRE(weight == profile.weight());
      REQUIRE(height == profile.height());
      REQUIRE(sex == profile.biologicalSex());
      REQUIRE(email == profile.email());
   }


   TEST_CASE("test_HeartRateMeasurement_bindings", "[binding]")
   {
      // data provider for unit tests that gets json from disk files.
      auto data_prov = TestDataProvider{ fs::path{"./test_data" } };

      HeartRateDataSeries hr_data{ detail::getDataSeries<HeartRate>(data_prov, detail::SortedPropertyMap{}) };

      REQUIRE(hr_data.size() > 0);

      auto&& hr = hr_data[0];
      auto& [bpm, source, timestamp] = hr;

      REQUIRE(bpm == hr.beatsPerMin());
      REQUIRE(source == hr.source());
      REQUIRE(timestamp == hr.timestamp());
   }

} // namespace oura_charts::test
