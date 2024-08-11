

#include "oura_charts/SchemaManager.h"
#include "oura_charts/detail/json_structs.h"

#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)


   TEST_CASE("test_schema_heart_rate", "[parsing][schema]")
   {
      using namespace schema;

      fs::path cwd = std::filesystem::current_path();
      SchemaManager mgr{};
      mgr.loadSchema("./schema");

      auto& schema = mgr.getClassSchema("HeartRate");

      REQUIRE(schema.class_name == "HeartRate");
      REQUIRE(schema.supported_groupings.size() == 1);
      REQUIRE(schema.fields.size() == 4);

      {
         auto& field = schema.fields[0];
         REQUIRE(field.class_member == "beatsPerMin");
         REQUIRE(field.long_display_name == "Beats per Minute");
         REQUIRE(field.short_display_name == "bpm");
         REQUIRE(field.supported_aggregates.size() == 5);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Count");
         REQUIRE(field.supported_aggregates[3] == "Sum");
         REQUIRE(field.supported_aggregates[4] == "Avg");
      }

      {
         auto& field = schema.fields[1];
         REQUIRE(field.class_member == "source");
         REQUIRE(field.long_display_name == "Measurement Source");
         REQUIRE(field.short_display_name == "Source");
         REQUIRE(field.supported_aggregates.size() == 5);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Count");
         REQUIRE(field.supported_aggregates[3] == "Sum");
         REQUIRE(field.supported_aggregates[4] == "Avg");
      }

      {
         auto& field = schema.fields[2];
         REQUIRE(field.class_member == "timestamp");
         REQUIRE(field.long_display_name == "Measurement Timestamp");
         REQUIRE(field.short_display_name == "Timestamp");
         REQUIRE(field.supported_aggregates.empty());
      }
      {
         auto& field = schema.fields[3];
         REQUIRE(field.class_member == "date");
         REQUIRE(field.long_display_name == "Measurement Date");
         REQUIRE(field.short_display_name == "Date");
         REQUIRE(field.supported_aggregates.empty());
      }
   }



   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

} // namespace oura_charts::test
