#pragma once

#include "oura_charts/oura_charts.h"

#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <ranges> 


//namespace oura_charts::constants
//{
//   inline constexpr const char* SCHEMA_DATACLASS_SLEEP_SESSION = "Sleep Session";
//   inline constexpr const char* SCHEMA_DATACLASS_DAILY_SLEEP_SCORE = "Daily Sleep Score";
//   inline constexpr const char* SCHEMA_DATACLASS_HEART_RATE = "Heart Rate";
//   inline constexpr const char* SCHEMA_AGGREGATE_FUNC_MIN = "Minimum";
//   inline constexpr const char* SCHEMA_AGGREGATE_FUNC_MAX = "Maximum";
//   inline constexpr const char* SCHEMA_AGGREGATE_FUNC_COUNT = "Count";
//   inline constexpr const char* SCHEMA_AGGREGATE_FUNC_SUM = "Sum";
//   inline constexpr const char* SCHEMA_AGGREGATE_FUNC_AVG = "Average";
//
//} // oura_charts::constants


namespace oura_charts::detail
{
   using std::vector;
   using std::string;
   using std::map;
   namespace fs = std::filesystem;

  

   /// <summary>
   ///   class to encapsulate schema information for a single member/field in a data object.
   /// </summary>
   struct FieldSchema
   {
      string         class_member{};
      string         long_display_name{};
      string         short_display_name{};
      vector<string> supported_aggregates{};
   };


   /// <summary>
   ///   encapsulates schema for group-by capabilities of a data object.
   /// </summary>
   struct GroupBySchema
   {
      string field{};
      string data_type{};
      vector<string> group_by;
   };


   /// <summary>
   ///   encapsulates schema information for a data object
   /// </summary>
   struct ClassSchema
   {
      string                  class_name{};
      vector<FieldSchema>    fields{};
      vector<GroupBySchema> supported_groupings{};
   };








   class SchemaManager
   {
   public:
      using SchemaMap = std::map<string, ClassSchema, std::less<>>;
      using Iter = SchemaMap::const_iterator;

      /// <summary>
      ///   Iterate over the schema map
      /// </summary>
      Iter begin() const { return m_class_schemas.begin(); }
      Iter end() const   { return m_class_schemas.end();   }


      /// <summary>
      ///   return the schema classes as range of map elements.
      /// </summary>
      auto getSchemaRange() const
      {
         return std::ranges::subrange{ begin(), end() };
      }


      /// <summary>
      ///   get a specific schema class by name. Will throw exception if the requested
      ///   class_name is invalid. class names are case-sensitive.
      /// </summary>
      const ClassSchema& getClassSchema(std::string_view class_name) const noexcept(false);


      /// <summary>
      ///   Load schema information from JSON files on disk. You can pass a single file path,
      ///   or a folder path in which case all *.json files will be loaded.
      /// </summary>
      void loadSchema(const fs::path& schema_path);


   private:
      SchemaMap m_class_schemas;

      void loadSchemaFile(const fs::path& file_path);
   };


#pragma region temp

   //enum class DataClass
   //{
   //   SleepSession = 0u,
   //   SleepScore,
   //   HeartRate
   //};

   //constexpr auto std::array DataClassNames = {
   //   SCHEMA_DATACLASS_SLEEP_SESSION,
   //   SCHEMA_DATACLASS_DAILY_SLEEP_SCORE,
   //   SCHEMA_DATACLASS_HEART_RATE
   //};


   //enum class AggregrateFunc
   //{
   //   Avg = 0u;
   //   Count,
   //   Min,
   //   Max,
   //   Sum,
   //   Any   // special value used to indicate a field supports all AggregateFuncs,
   //         // also serves as the upper limit for enum values, which are used as array indices
   //};


   //constexpr auto std::array AggregrateFuncNames = {
   //   SCHEMA_AGGREGATE_FUNC_AVG,
   //   SCHEMA_AGGREGATE_FUNC_COUNT,
   //   SCHEMA_AGGREGATE_FUNC_MIN,
   //   SCHEMA_AGGREGATE_FUNC_MAX,
   //   SCHEMA_AGGREGATE_FUNC_SUM
   //};


   //enum class PropFilter
   //{
   //   Equal = 0u,
   //   NotEqual,
   //   LessThan,
   //   GreaterThan
   //};


   //template<DataSeriesElement ElemT, typename PropT, template <typename T> typename AggregateT >
   //struct ClassProperty
   //{
   //   using ElemType = ElemT;
   //   using PropType = PropT;
   //   using AggregateFunc = AggregateT<PropT>;
   //   using MemberFunc = decltype(PropType(ElemType::*)() const);

   //   std::string name{};
   //   MemberFunc  member_func{};
   //   AggregateFunc aggregate_func{};

   //};
#pragma endregion


} // namespace oura_charts::schema
