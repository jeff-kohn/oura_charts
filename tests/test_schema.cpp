

#include "oura_charts/SchemaManager.h"
#include "oura_charts/detail/json_structs.h"

#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

   
   TEST_CASE("test_schema_heart_rate", "[parsing][schema]")
   {
      using namespace detail;

      SchemaManager mgr{};
      mgr.loadSchema("./schema");

      auto& schema = mgr.getClassSchema("HeartRate");
      
      REQUIRE(schema.class_name == "HeartRate");
      REQUIRE(schema.fields.size() == 4);


      {
         auto& field = schema.fields[0];
         REQUIRE(field.class_member == "beatsPerMin");
         REQUIRE(field.data_type == DataTypeSelection::Numeric);
         REQUIRE(field.supported_filters[0] == FilterSelection::Equal);
         REQUIRE(field.supported_filters[1] == FilterSelection::NotEqual);
         REQUIRE(field.supported_filters[2] == FilterSelection::LessThan);
         REQUIRE(field.supported_filters[3] == FilterSelection::LessOrEqual);
         REQUIRE(field.supported_filters[4] == FilterSelection::GreaterThan);
         REQUIRE(field.supported_filters[5] == FilterSelection::GreaterOrEqual);
      }

      {
         auto& field = schema.fields[1];
         REQUIRE(field.class_member == "source");
         REQUIRE(field.data_type == DataTypeSelection::String);
         REQUIRE(field.supported_filters[0] == FilterSelection::Equal);
         REQUIRE(field.supported_filters[1] == FilterSelection::NotEqual);
      }

      {
         auto& field = schema.fields[2];
         REQUIRE(field.class_member == "timestamp");
         REQUIRE(field.data_type == DataTypeSelection::DateTime);
         REQUIRE(field.supported_filters[0] == FilterSelection::Equal);
         REQUIRE(field.supported_filters[1] == FilterSelection::NotEqual);
         REQUIRE(field.supported_filters[2] == FilterSelection::LessThan);
         REQUIRE(field.supported_filters[3] == FilterSelection::LessOrEqual);
         REQUIRE(field.supported_filters[4] == FilterSelection::GreaterThan);
         REQUIRE(field.supported_filters[5] == FilterSelection::GreaterOrEqual);
      }
      {
         auto& field = schema.fields[3];
         REQUIRE(field.class_member == "date");
         REQUIRE(field.data_type == DataTypeSelection::Date);
         REQUIRE(field.supported_filters[0] == FilterSelection::Equal);
         REQUIRE(field.supported_filters[1] == FilterSelection::NotEqual);
         REQUIRE(field.supported_filters[2] == FilterSelection::LessThan);
         REQUIRE(field.supported_filters[3] == FilterSelection::LessOrEqual);
         REQUIRE(field.supported_filters[4] == FilterSelection::GreaterThan);
         REQUIRE(field.supported_filters[5] == FilterSelection::GreaterOrEqual);
      };
   }

   TEST_CASE("test_schema_daily_sleep_score", "[parsing][schema]")
   {
      using namespace detail;

      SchemaManager mgr{};
      mgr.loadSchema("./schema");

      auto& schema = mgr.getClassSchema("DailySleepScore");

      REQUIRE(schema.class_name == "DailySleepScore");
      REQUIRE(schema.fields.size() == 9);

      {
         auto& field = schema.fields[0];
         REQUIRE(field.class_member == "score");
      }
      {
         auto& field = schema.fields[1];
         REQUIRE(field.class_member == "date");
      }
      {
         auto& field = schema.fields[2];
         REQUIRE(field.class_member == "contributors.deep_sleep");
      }
      {
         auto& field = schema.fields[3];
         REQUIRE(field.class_member == "contributors.efficiency");
      }
      {
         auto& field = schema.fields[4];
         REQUIRE(field.class_member == "contributors.latency");
      }
      {
         auto& field = schema.fields[5];
         REQUIRE(field.class_member == "contributors.rem_sleep");
      }
      {
         auto& field = schema.fields[6];
         REQUIRE(field.class_member == "contributors.restfulness");
      }
      {
         auto& field = schema.fields[7];
         REQUIRE(field.class_member == "contributors.timing");
      }
      {
         auto& field = schema.fields[8];
         REQUIRE(field.class_member == "contributors.total_sleep");
      }
   }


   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

} // namespace oura_charts::test
