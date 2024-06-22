#include "TestDataProvider.h"
#include "oura_charts/detail/utility.h"
#include <boost/algorithm/string.hpp>
#include <fstream>


namespace oura_charts::test
{
   using namespace detail;

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
   [[nodiscard]] TestDataProvider::JsonResult TestDataProvider::getJsonData(std::string_view path, std::string_view next_token) const noexcept
   {
      std::string json_id{ path };
      json_id.append(next_token);

      auto it = m_json_map.find(json_id);
      if (it == m_json_map.end())
         return unexpected{ oura_exception{ErrorCategory::Parse, "JSON data not found for path '{}'", path } };

      // either return the string, or if we got a path load the file's contents and return that.
      auto visitor = overload{ [](auto&& json) -> JsonResult                     {  return std::string{ json };      },
                               [this] (const fs::path& json_path) -> JsonResult  {  return getJsonFile(json_path);   }
      };
      return std::visit(visitor, it->second);
   }


   TestDataProvider::JsonResult TestDataProvider::getJsonFile(const fs::path& file_path)  const noexcept
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


   void TestDataProvider::paginateDataSource(std::string_view json_path, size_t num_pages) noexcept(false)
   {
      constexpr std::string_view token_fmt_str{ R"("next_token": "{}")" };
      constexpr std::string_view null_token_str{ R"("next_token": null)" };

      auto result = getJsonData(json_path);
      if (!result)
         throw result.error();

      std::string original_json{ result.value() };
      auto token_pos = original_json.find(null_token_str);
      if (token_pos == std::string::npos)
      {
         auto func = std::source_location::current().function_name();
         throw oura_exception{ ErrorCategory::Parse, "{} - couldn't find next_token in JSON data",  func };
      }

      // paginate our datasource by duplicating the file in-memory and adding next_token values.
      // We'll just use the loop counter for the next_token values.
      for (size_t idx{ 0 }; idx < num_pages; ++idx)
      {
         std::string json_text{ original_json }; // always start with unmodified original
         auto next_idx = idx + 1;
         if (next_idx < num_pages)
         {
            json_text.replace(token_pos, null_token_str.size(), fmt::format(token_fmt_str, idx + 1));
         }
         std::string id_str = (idx == 0) ? "" : std::to_string(idx);
         addJsonData(std::string{ json_path }, json_text, id_str, true);
      }
   }

} // namespace oura_charts::test
