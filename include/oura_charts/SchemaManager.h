#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/aggregate_functors.h"

#include "glaze/glaze.hpp"

#include <filesystem>
#include <map>
#include <ranges> 
#include <string>
#include <string_view>
#include <vector>


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


   enum class FilterSelection
   {
      Null,
      NotNull,
      Equal,
      NotEqual,
      LessThan,
      LessOrEqual,
      GreaterThan,
      GreaterOrEqual
   };

   enum class DataTypeSelection
   {
      Numeric,
      Date,
      DateTime,
      TimeInterval,
      SleepType,
      String
   };

   /// <summary>
   ///   class to encapsulate schema information for a single member/field in a data object.
   /// </summary>
   struct FieldSchema
   {
      string                     class_member{};
      DataTypeSelection          data_type{};
      vector<FilterSelection>    supported_filters{};
   };

   /// <summary>
   ///   encapsulates schema information for a data object
   /// </summary>
   struct ClassSchema
   {
      string                 class_name{};
      vector<FieldSchema>    fields{};

      auto getFilterableFields()
      {
         return vw::all(fields) | vw::filter([] (FieldSchema& schema) { return !schema.supported_filters.empty(); })
            | vw::as_const;
      }
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
      Iter end() const { return m_class_schemas.end(); }

      /// <summary>
      ///   return the schema classes as range of map elements.
      /// </summary>
      auto getSchemaRange() const
      {
         return rg::subrange{ begin(), end() };
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

} // namespace oura_charts::schema


template <>
struct glz::meta<oura_charts::detail::FilterSelection>
{
   using enum oura_charts::detail::FilterSelection;
   static inline constexpr auto value = enumerate(
      Null,
      NotNull,
      Equal,
      NotEqual,
      LessThan,
      LessOrEqual,
      GreaterThan,
      GreaterOrEqual
   );
};

template <>
struct glz::meta<oura_charts::detail::DataTypeSelection>
{
   using enum oura_charts::detail::DataTypeSelection;
   static inline constexpr auto value = enumerate(
      Numeric,
      Date,
      DateTime,
      TimeInterval,
      SleepType,
      String
   );
};

