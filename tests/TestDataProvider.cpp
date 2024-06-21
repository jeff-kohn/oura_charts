#include "TestDataProvider.h"
#include "oura_charts/detail/utility.h"
#include <boost/algorithm/string.hpp>
#include <fstream>


namespace oura_charts::test
{

   TestDataProvider::TestDataProvider(const fs::path& data_folder) 
   {
      enumerateJsonFromFolder(data_folder);
   }


   void TestDataProvider::enumerateJsonFromFolder(const fs::path& data_folder)
   {
      if (data_folder.empty() or !fs::exists(data_folder))
         return;

      for (const auto& dir_entry : fs::directory_iterator{ data_folder })
      {
         if (dir_entry.is_regular_file())
         {
            fs::path file_path{ dir_entry.path() };
            std::string id{ file_path.stem().generic_string() };
            m_json_map.emplace(std::move(id), std::move(file_path));
         }
      }
   }


   bool TestDataProvider::addJsonData(std::string_view path, std::string_view json_data, bool overwrite_existing)
   {
      return addJsonData(std::string{path}, std::string{json_data}, "", overwrite_existing);
   }


   bool TestDataProvider::addJsonData(std::string path, std::string json_data, std::string_view next_token, bool overwrite_existing)
   {
      boost::to_lower(path);
      path.append(next_token);

      auto it = m_json_map.find(path);
      if (it == m_json_map.end())
      {
         m_json_map.emplace(std::move(path), std::move(json_data));
         return true;
      }
      else if (overwrite_existing)
      {
         it->second = json_data;
         return true;
      }

      return false;
   }


   // clang-tidy thinks this method shouldn't be noexcept(), but I can't see how it would ever through short of memory allocation failing 
   // NOLINTNEXTLINE(bugprone-exception-escape)
   [[nodiscard]] TestDataProvider::JsonResult TestDataProvider::getJsonObject(std::string_view path, std::string_view next_token) const noexcept
   {
      using namespace detail;

      std::string json_id{ path };
      json_id.append(next_token);

      auto it = m_json_map.find(json_id);
      if (it == m_json_map.end())
         return unexpected{ oura_exception{ErrorCategory::Parse, "JSON data not found for path '{}'", path } };

      // either return the string, or if we got a path load the file's contents and return that.
      auto visitor = overload{ [](auto&& json) -> JsonResult                     {  return std::string{json};      },
                              [this] (const fs::path& json_path) -> JsonResult   {  return doFileGet(json_path);   }
      };

      return std::visit(visitor, it->second);
   }


   TestDataProvider::JsonResult TestDataProvider::doFileGet(const fs::path& file_path)  const noexcept
   {
      try
      {
         auto file_str = file_path.generic_string();
         if (!fs::exists(file_path))
         {
            return unexpected{ oura_exception{ ErrorCategory::FileIO, "JSON data file[{}] not found.",  file_str} };
         }

         std::ifstream json_file{ file_path };
         if (!json_file.bad())
            return std::string{ std::istreambuf_iterator<char>(json_file), std::istreambuf_iterator<char>() };
         else
            return unexpected{ oura_exception{ ErrorCategory::FileIO, "JSON data file[{}] could not be read.",  file_str} };
      }
      catch (std::exception& e)
      {
         return unexpected{ oura_exception{ e.what() } };
      }
   }




} // namespace oura_charts::test
