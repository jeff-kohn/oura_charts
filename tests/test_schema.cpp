

#include "oura_charts/SchemaManager.h"
#include "oura_charts/detail/json_structs.h"

#include <catch2/catch_test_macros.hpp>

namespace oura_charts::test
{
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)


   TEST_CASE("test_schema_heart_rate", "[parsing][schema]")
   {
      using namespace schema;

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

   TEST_CASE("test_schema_daily_sleep_score", "[parsing][schema]")
   {
      using namespace schema;

      fs::path cwd = std::filesystem::current_path();
      SchemaManager mgr{};
      mgr.loadSchema("./schema");

      auto& schema = mgr.getClassSchema("DailySleepScore");

      REQUIRE(schema.class_name == "DailySleepScore");
      REQUIRE(schema.supported_groupings.size() == 1);
      REQUIRE(schema.fields.size() == 9);

      {
         auto& field = schema.fields[0];
         REQUIRE(field.class_member == "score");
         REQUIRE(field.long_display_name == "Sleep Score");
         REQUIRE(field.short_display_name == "score");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }

      {
         auto& field = schema.fields[1];
         REQUIRE(field.class_member == "date");
         REQUIRE(field.long_display_name == "Measurement Date");
         REQUIRE(field.short_display_name == "date");
         REQUIRE(field.supported_aggregates.empty());
      }


      {
         auto& field = schema.fields[2];
         REQUIRE(field.class_member == "contributors.deep_sleep");
         REQUIRE(field.long_display_name == "Deep Sleep Contribution");
         REQUIRE(field.short_display_name == "deep sleep contrib.");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }
      {
         auto& field = schema.fields[3];
         REQUIRE(field.class_member == "contributors.efficiency");
         REQUIRE(field.long_display_name == "Sleep Efficiency Contribution");
         REQUIRE(field.short_display_name == "efficiency contrib.");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }
      {
         auto& field = schema.fields[4];
         REQUIRE(field.class_member == "contributors.latency");
         REQUIRE(field.long_display_name == "Sleep Latency Contribution");
         REQUIRE(field.short_display_name == "latency contrib.");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }
      {
         auto& field = schema.fields[5];
         REQUIRE(field.class_member == "contributors.rem_sleep");
         REQUIRE(field.long_display_name == "REM Sleep Contribution");
         REQUIRE(field.short_display_name == "rem sleep contrib.");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }
      {
         auto& field = schema.fields[6];
         REQUIRE(field.class_member == "contributors.restfulness");
         REQUIRE(field.long_display_name == "Restfulness Contribution");
         REQUIRE(field.short_display_name == "restfulness contrib.");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }
      {
         auto& field = schema.fields[7];
         REQUIRE(field.class_member == "contributors.timing");
         REQUIRE(field.long_display_name == "Sleep Timing Contribution");
         REQUIRE(field.short_display_name == "sleep timing contrib.");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }
      {
         auto& field = schema.fields[8];
         REQUIRE(field.class_member == "contributors.total_sleep");
         REQUIRE(field.long_display_name == "Total Sleep Contribution");
         REQUIRE(field.short_display_name == "total sleep contrib.");
         REQUIRE(field.supported_aggregates.size() == 4);
         REQUIRE(field.supported_aggregates[0] == "Min");
         REQUIRE(field.supported_aggregates[1] == "Max");
         REQUIRE(field.supported_aggregates[2] == "Sum");
         REQUIRE(field.supported_aggregates[3] == "Avg");
      }
   }


   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

} // namespace oura_charts::test
