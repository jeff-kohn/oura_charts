#include "oura_charts/SchemaManager.h"
#include "oura_charts/oura_exception.h"
#include <glaze/glaze.hpp>
#include <boost/algorithm/string.hpp>

namespace oura_charts::schema
{

   void SchemaManager::loadSchema(const fs::path& schema_path)
   {
      if (!fs::is_regular_file(schema_path))
      {
         // just load a single file.
         loadSchemaFile(schema_path);
         return;
      }

      for (auto& entry : fs::directory_iterator{ schema_path })
      {
         // folder was passed, so recurse all json files and load them (non-recursively)
         if (fs::is_regular_file(entry))
         {
            auto file = entry.path();
            if (boost::iends_with(file.generic_string(), constants::SCHEMA_FILE_EXT))
               loadSchemaFile(entry.path());
         }
      }
   }


   void SchemaManager::loadSchemaFile(const fs::path& file_path)
   {
      ClassSchema schema{};
      std::string buffer{};
      auto ec = glz::read_file_json(schema, file_path.generic_string(), buffer);
      if (ec)
          throw oura_exception{ static_cast<int64_t>(ec.ec), glz::format_error(ec, buffer), ErrorCategory::Parse };

      m_class_schemas[schema.class_name] = std::move(schema);
   }


   const ClassSchema& SchemaManager::getClassSchema(std::string_view class_name) const
   {
      auto it = m_class_schemas.find(class_name);
      if (it == m_class_schemas.end())
         throw oura_exception{ ErrorCategory::Generic, constants::FMT_MSG_ERROR_SCHEMA_CLASS_NOT_FOUND, class_name };

      return it->second;
   }


} // namespace oura_charts::schema
