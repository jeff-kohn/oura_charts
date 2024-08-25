#include "oura_charts/DailySleepScoreQuery.h"
#include "oura_charts/functors.h"
#include "frozen/map.h"
#include <functional>
#include <map>
#include <variant>


namespace oura_charts
{
   //using TypedAggregate = std::pair<AggregateFuncType, DataType>;

   //template<typename AggrT, typename ValT>
   //using AggregateFactory = std::function<AggrT<T>()>;

   //constinit Overloaded AggregateFactory{
   //   [](AggregateFuncType func_type, DataType member_type, DataType result_type)
   //   {

   //   }
   //};

   //template<typename T>
   //class AggregateBase
   //{
   //   virtual void operator()(const DailySleepScore& score) noexcept = 0;
   //};


   int getContribution(const DailySleepScore& data, SleepScoreQuery::MemberFields member)
   {
      using enum SleepScoreQuery::MemberFields;

      const auto& contribs = data.contributors();
      switch (member)
      {
         case contrib_deep_sleep:
            return contribs.deep_sleep;
         case contrib_efficiency:
            return contribs.efficiency;
         case contrib_latency:
            return contribs.latency;
         case contrib_rem:
            return contribs.rem_sleep;
         case contrib_restfulness:
            return contribs.restfulness;
         case contrib_sleep_timing:
            return contribs.timing;
         case contrib_total_sleep:
            return contribs.total_sleep;
         default:
            throw std::invalid_argument{ "Invalid member requested" };
      }
   }

   using namespace std::placeholders;
   using MemFun = decltype((std::bind(&DailySleepScore::score, _1)));
   using MemFunNested = decltype((std::bind(getContribution, _1)));

   using MemVarT = std::variant<MemFun, MemFunNested>;
   using MemberMap = frozen::map<SleepScoreQuery::MemberFields, MemVarT, 7>;
   static inline constexpr MemberMap member_map
   {
      {SleepScoreQuery::MemberFields::score,                MemVarT{MemFun{std::bind(&DailySleepScore::score, _1)}}},
      {SleepScoreQuery::MemberFields::contrib_deep_sleep,   MemVarT{MemFunNested{std::bind(getContribution, _1)}}},
      {SleepScoreQuery::MemberFields::contrib_latency,      MemVarT{MemFunNested{std::bind(getContribution, _1)}}},
      {SleepScoreQuery::MemberFields::contrib_rem,          MemVarT{MemFunNested{std::bind(getContribution, _1)}}},
      {SleepScoreQuery::MemberFields::contrib_restfulness,  MemVarT{MemFunNested{std::bind(getContribution, _1)}}},
      {SleepScoreQuery::MemberFields::contrib_sleep_timing, MemVarT{MemFunNested{std::bind(getContribution, _1)}}},
      {SleepScoreQuery::MemberFields::contrib_total_sleep,  MemVarT{MemFunNested{std::bind(getContribution, _1)}}}
   };

   //inline constexpr std::map<AggregateFunc, 
   void SleepScoreQuery::RunQuery(DailySleepScoreSeries& data_series)
   {
      std::vector<AggregrateFuncV<int, double>> funcs(m_query_fields.size());

      for (QueryField fld : m_query_fields)
      {
         auto vt_fn = member_map.find(fld.member);
         assert(vt_fn != member_map.end());
      }

   }

} // namespace oura_charts

