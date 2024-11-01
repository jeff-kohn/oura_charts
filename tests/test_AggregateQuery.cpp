#include "TestDataProvider.h"
#include "oura_charts/AggregateQuery.h"
#include "oura_charts/DailySleepScoreTraits.h"
#include "oura_charts/SelectQuery.h"
#include "oura_charts/HeartRateTraits.h"
#include "oura_charts/detail/nullable_types.h"

#include <catch2/catch_test_macros.hpp>

#include <string_view>

namespace oura_charts::test
{
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

   using namespace oura_charts::detail;
   using namespace std::literals::string_view_literals;

   constexpr auto score_json = R"(
      {
        "data": [
          {
            "id": "1",
            "contributors": {
              "deep_sleep": 20,
              "efficiency": 30,
              "latency": 40,
              "rem_sleep": 50,
              "restfulness": 60,
              "timing": 70,
              "total_sleep": 80
            },
            "day": "2024-06-03",
            "score": 90,
            "timestamp": "2024-06-03T02:37:55+00:00"
          },
          {
            "id": "2",
            "contributors": {
              "deep_sleep": 21,
              "efficiency": 31,
              "latency": 41,
              "rem_sleep": 51,
              "restfulness": 61,
              "timing": 71,
              "total_sleep": 81
            },
            "day": "2024-06-03",
            "score": 91,
            "timestamp": "2024-06-03T02:37:55+00:00"
          },
          {
            "id": "3",
            "contributors": {
              "deep_sleep": 22,
              "efficiency": 32,
              "latency": 42,
              "rem_sleep": 52,
              "restfulness": 62,
              "timing": 72,
              "total_sleep": 82
            },
            "day": "2024-06-03",
            "score": 92,
            "timestamp": "2024-06-03T02:37:55+00:00"
          }
        ],
        "next_token": null
      }
   )"sv;

   TEST_CASE("test_AggregateQuery", "[parsing][schema]")
   {
      TestDataProvider provider{};
      provider.addJsonData(constants::REST_PATH_DAILY_SLEEP, score_json);

      using ScoreQuery = SelectQuery<DailySleepScoreTraits>;
      ScoreQuery query{};
      query.addField(ScoreQuery::Field{ DailySleepScoreTraits::PropertySelection::score });
      query.addFilter(ScoreQuery::Filter{
         DailySleepScoreTraits::PropertySelection::score,
         [] (const ScoreQuery::FieldValueVt& val_vt) -> bool
         {
            auto* val = get_if<detail::NullableInt>(&val_vt);
            if (val)
               return val->has_value() ? (**val < 70) : false;

            return false;
         }
      });


   };

  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
} // namespace oura_charts::test
