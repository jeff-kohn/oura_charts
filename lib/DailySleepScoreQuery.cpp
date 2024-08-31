#include "oura_charts/DailySleepScoreQuery.h"
#include "oura_charts/functors.h"
#include "oura_charts/SchemaManager.h"
#include "frozen/map.h"
#include <functional>
#include <map>
#include <variant>


namespace oura_charts
{
   namespace detail
   {

      int getContribution(const DailySleepScore& data, SleepScoreQuery::Member member)
      {
         using enum SleepScoreQuery::Member;

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

      /// <summary>
      ///   functor to retrieve the score from a DailySleepScore object.
      /// </summary>
      struct ScoreFunc
      {
         using ResultType = int;
         ResultType operator()(const DailySleepScore& score) const
         {
            return score.score();
         }
      };


      /// <summary>
      ///   functor to retrieve a member from the nested Contributors structs of a DailySleepScore
      /// </summary>
      struct ContribFunc
      {
         SleepScoreQuery::Member member{};

         using ResultType = int;
         ResultType operator()(const DailySleepScore& score) const
         {
            return getContribution(score, member);
         }
      };

      /// <summary>
      ///   Map to get a member functor from a member enum
      /// </summary>
      using MemberMap = frozen::map<SleepScoreQuery::Member, MemberV, 7>;
      static inline constexpr MemberMap member_map
      {
         {SleepScoreQuery::Member::score,                MemberV{ScoreFunc{}}},
         {SleepScoreQuery::Member::contrib_deep_sleep,   MemberV{ContribFunc{SleepScoreQuery::Member::contrib_deep_sleep}}},
         {SleepScoreQuery::Member::contrib_latency,      MemberV{ContribFunc{SleepScoreQuery::Member::contrib_latency}}},
         {SleepScoreQuery::Member::contrib_rem,          MemberV{ContribFunc{SleepScoreQuery::Member::contrib_rem}}},
         {SleepScoreQuery::Member::contrib_restfulness,  MemberV{ContribFunc{SleepScoreQuery::Member::contrib_restfulness}}},
         {SleepScoreQuery::Member::contrib_sleep_timing, MemberV{ContribFunc{SleepScoreQuery::Member::contrib_sleep_timing}}},
         {SleepScoreQuery::Member::contrib_total_sleep,  MemberV{ContribFunc{SleepScoreQuery::Member::contrib_total_sleep}}}
      };

      /// <summary>
      ///   helper function to lookup up a member functor from a member enum. Since variant<> default
      ///   initializes to its first argument, this function returns a ScoreFunc if the requested enum
      ///   wasn't found in the map (which shouldn't be possible and indicates a bug.
      /// </summary>
      MemberV getMember(SleepScoreQuery::Member member)
      {
         auto mem_var = member_map.find(member);
         bool found{member_map.end() == mem_var}; assert(found);

         return found ? MemberV{} : mem_var->second;
      }

   } // namespace detail;

   using namespace detail;


   SleepScoreQuery::QueryField::QueryField(Member member, Aggregate aggregate) : m_member(member), m_aggregate(aggregate)
   {
      setMember(member);
      setAggregate(aggregate);
   }


   SleepScoreQuery::QueryField::QueryField(const QueryField& other) :
      m_member{ other.m_member },
      m_aggregate{ other.m_aggregate },
      m_aggregate_func{ other.m_aggregate_func }
   {
      m_member_func = std::make_unique<MemberV>(detail::getMember(m_member));
   }


   SleepScoreQuery::QueryField& SleepScoreQuery::QueryField::operator=(const QueryField& other)
   {
      if (this == &other)
         return *this;

      QueryField copy{ other };
      *this = std::move(other);
      return *this;
   }


   SleepScoreQuery::QueryField& SleepScoreQuery::QueryField::setMember(Member member)
   {
      m_member = member;
      m_member_func = std::make_unique<MemberV>(detail::getMember(m_member));
      return *this;
   }


   SleepScoreQuery::QueryField& SleepScoreQuery::QueryField::setAggregate(Aggregate aggregate)
   {
      // we don't check for setting the same aggregate, because even if it's the same
      // we want a new instance.
      m_aggregate = aggregate;
      m_aggregate_func = getAggregateFunctor<int>(m_aggregate);
      return *this;
   }


   void SleepScoreQuery::QueryField::operator()(const DailySleepScore& score)
   {
      if(m_member_func.get())
      {
         std::visit([&score] (auto&& func)
                    {
                       func(score);
                    },
                    *m_member_func);
      }
      else
      {  // should never happen
         assert(false);
         std::unreachable();
      }
   }


   FieldValueVt SleepScoreQuery::QueryField::getResult() const
   {
      return std::visit([] (auto&& func)
                        {
                           auto val = func.result();
                           return FieldValueVt(val);
                        },
                        m_aggregate_func);
   }


   void SleepScoreQuery::QueryField::clearResult()
   {
      // just reset the functor
      setAggregate(m_aggregate);
   }


   void SleepScoreQuery::runQuery(const DailySleepScoreSeries& series)
   {
      auto row_func = [this](const DailySleepScore& score)
                        {
                           rg::for_each(m_fields, [&score](QueryField& fld){ fld(score); });
                        };

      rg::for_each(series, row_func);
   }

   void SleepScoreQuery::clearResults()
   {
      rg::for_each(m_fields, [](auto&& fld) { fld.clearResult(); });
   }



} // namespace oura_charts

