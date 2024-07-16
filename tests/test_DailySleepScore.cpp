#include "TestDataProvider.h"
#include "oura_charts/DailySleepScore.h"
#include "oura_charts/chrono_helpers.h"
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <set>

namespace oura_charts::test
{
   using namespace std::literals;
   using namespace detail;

   // These tests SHOULD crash if they improperly access an empty optional<>
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access)

   auto getSleepScore(const daily_sleep_data& data, local_seconds now)
   {
      constexpr auto json_fmt = R"({{
         "data": [
         {{
            "id": "{}",
               "contributors" : {{
                  "deep_sleep": {},
                  "efficiency" : {},
                  "latency" : {},
                  "rem_sleep" : {},
                  "restfulness" : {},
                  "timing" : {},
                  "total_sleep" : {}
            }},
               "day": "{}",
               "score" : {},
               "timestamp" : "{}"
         }}
         ],
            "next_token": null
      }})";

      const auto json_text = fmt::format(json_fmt, data.id, data.contributors.deep_sleep, data.contributors.efficiency, data.contributors.latency,
                                             data.contributors.rem_sleep, data.contributors.restfulness, data.contributors.timing, data.contributors.total_sleep,
                                             toIsoDate(getCalendarDate(now)), data.score, toIsoDateTime(now));

     return readJson<RestDataCollection<DailySleepScore::StorageType>>(json_text);
   }

   TEST_CASE("test_DailySleepScore_parse")
   {
      auto now = floor<seconds>(localNow());
      const daily_sleep_data data{ "1", getCalendarDate(now), 75, { 10, 20, 30, 40, 50, 60, 70 },  now };

      auto data_res = getSleepScore(data, now);
      REQUIRE(data_res.has_value());

      auto& parsed_data = data_res->data;
      REQUIRE(parsed_data.size() == 1);

      DailySleepScore score{ std::move(parsed_data[0]) };
      REQUIRE(score.id() == data.id);
      REQUIRE(score.date() == data.day);
      REQUIRE(score.score() == data.score);
      REQUIRE(score.contributors().deep_sleep == data.contributors.deep_sleep);
      REQUIRE(score.contributors().efficiency == data.contributors.efficiency);
      REQUIRE(score.contributors().latency == data.contributors.latency);
      REQUIRE(score.contributors().rem_sleep == data.contributors.rem_sleep);
      REQUIRE(score.contributors().restfulness == data.contributors.restfulness);
      REQUIRE(score.contributors().timing == data.contributors.timing);
      REQUIRE(score.contributors().total_sleep == data.contributors.total_sleep);
   }

   TEST_CASE("test_DailySleepScore_bindings")
   {
      auto now = floor<seconds>(localNow());
      const daily_sleep_data data{ "1", getCalendarDate(now), 75, { 10, 20, 30, 40, 50, 60, 70 },  now };

      auto data_res = getSleepScore(data, now);
      REQUIRE(data_res.has_value());

      auto& parsed_data = data_res->data;
      REQUIRE(parsed_data.size() == 1);

      DailySleepScore daily_score{ parsed_data[0] };
      auto&& [id, day, score, contribs, ts] = daily_score;
      REQUIRE(id == daily_score.id());
      REQUIRE(day == daily_score.date());
      REQUIRE(score == daily_score.score());
      REQUIRE(contribs.deep_sleep == daily_score.contributors().deep_sleep);
      REQUIRE(ts == daily_score.timestamp());
   }

   // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, bugprone-unchecked-optional-access)


}// namespace oura_charts::test

