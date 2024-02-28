#pragma once
#include "oura_charts/constants.h"
#include <cpr/error.h>
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
      Parse
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
      case ErrorCategory::Parse:
         return constants::ERROR_CATEGORY_PARSE;
      default:
         std::unreachable();
      }
   }

   /// <summary>
   ///   formatter for ErrorCategory enum.
   /// </summary>
   inline std::string format_as(ErrorCategory ec)
   {
      return getGetogoryName(ec);
   }


   /// <summary>
   ///   exception class used by the Oura Charts project.
   ///
   ///   A default-constructed instance of this class will have an error category
   ///   of "Success" with no other values, essentially representing a non-error.
   /// </summary>
   /// <remarks>
   ///   This class is also used to return error information via std::excpected
   ///   when throwing exceptions is not desirable.
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
                                                                                  category{ category }
      {
      }

      oura_exception(std::string error_text, ErrorCategory category = ErrorCategory::Generic)
         : message{ std::move(error_text) },
           category{ category },
           error_code{ -1 }
      {
      }

      oura_exception() = default;
      oura_exception(const oura_exception &) = default;
      oura_exception(oura_exception &&) = default;
      oura_exception& operator=(const oura_exception&) = default;
      oura_exception& operator=(oura_exception&&) = default;
      ~oura_exception() override = default;

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
      return fmt::format("{} {} (0x{:X}): {}",
                         getGetogoryName(err.category),
                         err.error_code,
                         err.error_code,
                         err.what());
   }

}
