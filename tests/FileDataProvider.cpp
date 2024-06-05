#include "FileDataProvider.h"
#include <fstream>

namespace oura_charts::test
{

   FileDataProvider::FileDataProvider(fs::path data_folder) : m_data_folder(data_folder)
   {
      if (!fs::exists(m_data_folder))
         throw fs::filesystem_error{ "Data directory not found for file-based JSON data provider.", data_folder,
                                      std::make_error_code(std::errc::no_such_file_or_directory) };
   }

   FileDataProvider::expected_json FileDataProvider::doFileGet(fs::path filename)  const noexcept
   {
      try
      {
         if (!filename.has_extension())
            filename.replace_extension(".json");

         auto file_path = m_data_folder / filename;
         auto file_str = file_path.generic_string();
         if (!fs::exists(file_path))
         {
            auto msg = fmt::format("", file_path.generic_string());
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


}
