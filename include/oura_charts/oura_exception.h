//---------------------------------------------------------------------------------------------------------------------
// oura_exception.h
//
// Defines the oura_exception class, which is used to report errors via throw() and expected<>
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

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
      Parse,
      FileIO
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
      case ErrorCategory::FileIO:
         return constants::ERROR_CATEGORY_FILE_IO;
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
   ///   This class is also used to return error information via std::expected<>
   ///   when throwing exceptions is not desirable.
   /// </remarks>
   class oura_exception final : public std::exception
   {
   public:
      static constexpr int64_t ERROR_CODE_GENERAL_FAILURE = -1;

      /// <summary>
      ///   any error code associated with this error, zero if unspecified
      /// </summary>
      int64_t error_code{};

      /// <summary>
      ///   description of the error that occurred
      /// </summary>
      std::string message{};

      /// <summary>
      ///   category of error that occured
      /// </summary>
      ErrorCategory category{ ErrorCategory::Success };

      /// <summary>
      ///   textual name of the ErrorCategory
      /// </summary>
      std::string categoryName() const
      {
         return getGetogoryName(category);
      }

      /// <summary>
      ///   base class override, just returns the contents of message.
      /// </summary>
      const char* what() const noexcept override
      {
         return message.c_str();
      }


      /// <summary>
      ///   construct an exception from a cpr::Error
      /// </summary>
      explicit oura_exception(cpr::Error err) : error_code{ static_cast<int64_t>(err.code) },
                                                message{ std::move(err.message) },
                                                category{ ErrorCategory::REST }
      {
      }

      /// <summary>
      ///   create an exception from an error code, error message, and category
      /// </summary>
      oura_exception(int64_t code, std::string message, ErrorCategory category) : error_code{ static_cast<int64_t>(code) },
                                                                                  message{ std::move(message) },
                                                                                  category{ category }
      {
      }


      /// <summary>
      ///   create an exception from an error message and optional category
      /// </summary>
      oura_exception(std::string error_text, ErrorCategory category = ErrorCategory::Generic)
         : message{ std::move(error_text) },
           category{ category },
           error_code{ ERROR_CODE_GENERAL_FAILURE }
      {
      }

      /// <summary>
      ///   This ctor forwards the parameter pack to format() for the error message.
      /// </summary>
      template <typename... T>
      constexpr oura_exception(ErrorCategory errcat, fmt::format_string<T...> fmt, T&&... args)
         : category{ category },
           error_code{ ERROR_CODE_GENERAL_FAILURE }
      {
         message = fmt::format(fmt, args...);
      }

      oura_exception() = default;
      oura_exception(const oura_exception&) = default;
      oura_exception(oura_exception&&) = default;
      oura_exception& operator=(const oura_exception&) = default;
      oura_exception& operator=(oura_exception&&) = default;
      ~oura_exception() override = default;\
   };


   /// <summary>
   /// custom formatter for our exception
   /// </summary>
   inline auto format_as(const oura_exception& err)
   {
      return fmt::format("{} {} (0x{:X}): {}",
                         err.categoryName(),
                         err.error_code,
                         err.error_code,
                         err.what());
   }

}
