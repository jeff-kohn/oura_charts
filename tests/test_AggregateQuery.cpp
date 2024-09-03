#include "TestDataProvider.h"
#include "oura_charts/AggregateQuery.h"
#include "oura_charts/DailySleepScoreTraits.h"

#include <catch2/catch_test_macros.hpp>

#include <string_view>

namespace oura_charts::test
{
   // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

   using namespace oura_charts::detail;
   using namespace std::literals::string_view_literals;

   //std::string id{};
   //year_month_day day{};
   //int score{};
   //SleepScoreContributors contributors{};
   //local_seconds timestamp{};

   constexpr auto score_json = R"({
      {
        "data": [
          {
            "id": "string",
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
          }
        ],
          {
            "id": "string",
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
          }
        ],
          {
            "id": "string",
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
        "next_token": "string"
      }
   })"sv;


   TEST_CASE("test_AggregateQuery", "[parsing][schema]")
   {
      TestDataProvider provider{};
      provider.addJsonData(constants::REST_PATH_DAILY_SLEEP, score_json);

      using DailySleepScoreQuery = AggregateQuery<DailySleepScoreTraits>;
      DailySleepScoreQuery query{};
      query.m_fields.emplace_back(DailySleepScoreTraits::PropertySelection::score,
                                  detail::AggregateSelection::Avg);

      auto series = getDataSeries<DailySleepScore>(provider, year_month_day{ 2024y/06/03 }, year_month_day{ 2924y/06/03 });
      query.runQuery(series);

      auto val = query.m_fields[0].getResult();
      
   };

} // namespace oura_charts::test
