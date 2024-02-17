#pragma once
#include "oura_charts/constants.h"
#include <cpr/error.h>
#include <cpr/status_codes.h>
#include <fmt/format.h>
#include <string>

namespace oura_charts
{
   /// <summary>
   /// just a broad, general categorization of the types of errors we may encounter
   /// </summary>
   enum class ErrorCategory
   {
      Generic = -1,
      Success = 0,
      REST,
      JSON
   };

   inline std::string getGetogoryName(ErrorCategory ec)
   {
      switch (ec)
      {
      case ErrorCategory::Generic:
         return constants::ERROR_CATEGORY_GENERIC;
      case ErrorCategory::Success:
         return constants::ERROR_CATEGORY_SUCCESS;
      case ErrorCategory::REST:
         return constants::ERROR_CATEGORY_REST;
      case ErrorCategory::JSON:
         return constants::ERROR_CATEGORY_JSON;
      }
   }


   /// <summary>
   /// 
   /// exception class used by the Oura Charts project.
   /// 
   /// </summary>
   /// 
   /// <remarks>
   /// 
   /// This type is also used to report errors via return value in some cases
   /// with std::expected
   ///
   /// A default-constructed instance of this class will have an error category
   /// of "Success" with no other values, essentially representing a non-error.
   /// 
   /// </remarks>
   class oura_exception final : public std::exception
   {
   public:
      explicit oura_exception(cpr::Error err) : error_code{ static_cast<int64_t>(err.code) },
                                                message{ std::move(err.message) },
                                                category{ ErrorCategory::REST }
      {
      }

      oura_exception(int64_t code, std::string message, ErrorCategory category) : error_code{ static_cast<int64_t>(code) },
                                                                                  message{ std::move(message) },
                                                                                  category{ ErrorCategory::REST }
      {
      }

      oura_exception(std::string error_text, ErrorCategory category = ErrorCategory::Generic)
         : error_code{ -1 },
           message{ error_text },
           category{ ErrorCategory::Generic }
      {
      }

      oura_exception() = default;
      oura_exception(const oura_exception &) = default;
      oura_exception(oura_exception &&) = default;
      oura_exception& operator=(const oura_exception&) = default;
      oura_exception& operator=(oura_exception&&) = default;

      int64_t error_code{};
      std::string message{};
      ErrorCategory category{ ErrorCategory::Success };

      const char* what() const noexcept override 
      {
         return message.c_str();
      }
   };

   /// <summary>
   /// custom formatter for our exception
   /// </summary>
   inline auto format_as(const oura_exception& err)
   {
      return fmt::format("'{}' error 0x{:X} occurred: {}",
                         getGetogoryName(err.category),
                         err.error_code,
                         err.what());
   }

}


///// <summary>
///// custom formatter for our exception
///// </summary>
//template <>
//struct fmt::formatter<oura_charts::oura_exception> : fmt::formatter<std::string_view>
//{
//   auto format(const oura_charts::oura_exception& err, fmt::format_context& ctx) const
//   {
//      std::string temp;
//      fmt::format_to(std::back_inserter(temp), "'{}' error 0x{:X} occurred: {}",
//                                                oura_charts::getGetogoryName(err.category),
//                                                err.error_code,
//                                                err.what());
//
//      return fmt::formatter<std::string_view>::format(temp, ctx);
//   }
//};
