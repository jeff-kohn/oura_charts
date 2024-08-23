#include "oura_charts/DailySleepScoreQuery.h"
#include "oura_charts/functors.h"
#include <map>
#include <variant>


namespace oura_charts
{
   enum class DataType
   {
      Int32,
      Int64,
      Uint32,
      Uint64,
      Float,
      Double
   };

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



   //inline constexpr std::map<AggregateFunc, 
   void SleepScoreQuery::RunQuery(DailySleepScoreSeries& data_series)
   {
   }

} // namespace oura_charts

