//---------------------------------------------------------------------------------------------------------------------
// logging.h
//
// defines the logging namespace and interface for logging functionality.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/concepts.h"
#include "oura_charts/constants.h"
#include "oura_charts/oura_exception.h"
#include "oura_charts/detail/utility.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <filesystem>
#include <string_view>
#include <source_location>


/// <summary>
///   Namespace that contains the types, functions, and objects used for the logging interface of this project.
/// </summary>
namespace oura_charts::logging
{
   //
   // import these symbols into our namespace, so logging interface looks like logging::warn(...) and there's
   // possiblity to replace logging backend with minimal impact if needed.
   //
   namespace fs = std::filesystem;
   namespace sinks = spdlog::sinks;
   namespace log_level = spdlog::level;
   using spdlog::logger;
   using spdlog::source_loc;
   using spdlog::level::level_enum;
   using spdlog::sinks::daily_file_sink_mt;
   using spdlog::sinks::stdout_color_sink_mt;
   using spdlog::sinks::stderr_color_sink_mt;
   using spdlog::sinks::msvc_sink_mt;
   using spdlog::sinks_init_list;
   using spdlog::set_default_logger;
   using spdlog::log;
   using spdlog::error;
   using spdlog::warn;
   using spdlog::info;
   using spdlog::critical;
   using log_ptr_t = std::shared_ptr<logger>;
   using sink_ptr_t = sinks_init_list::value_type;

} // namespace oura_charts::log


/// <summary>
///    logging-related constants.
/// </summary>
namespace oura_charts::constants
{
   static constexpr const char* CONFIG_DEFAULT_LOG_NAME = "DEFAULT";
   static constexpr const char* CONFIG_DEFAULT_LOG_PATTERN_CONSOLE = "[%Y-%m-%d %H:%M:%S.%e] [TID %t] [%l] %v";
   static constexpr const char* CONFIG_DEFAULT_LOG_PATTERN_FILE = "[%Y-%m-%d %H:%M:%S.%e] [%s:%#] [%^%l%$] %v";
   static constexpr const char* CONFIG_DEFAULT_LOG_PATTERN_DEBUGGER = "[%Y-%m-%d %H:%M:%S.%e] [%s:%#] [%^%l%$] %v";

   #if !defined(NDEBUG)
      static constexpr auto CONFIG_DEFAULT_LOGLEVEL_DAILYFILE = oura_charts::logging::level_enum::trace;
      static constexpr auto CONFIG_DEFAULT_LOGLEVEL_CONSOLE = oura_charts::logging::level_enum::warn;
      static constexpr auto CONFIG_DEFAULT_LOGLEVEL_DEBUGGER = oura_charts::logging::level_enum::info;
   #else
      static constexpr auto CONFIG_DEFAULT_LOGLEVEL_DAILYFILE = oura_charts::logging::level_enum::warn;
      static constexpr auto CONFIG_DEFAULT_LOGLEVEL_CONSOLE = oura_charts::logging::level_enum::err;
      static constexpr auto CONFIG_DEFAULT_LOGLEVEL_DEBUGGER = oura_charts::logging::level_enum::off;
   #endif
} // namespace oura_charts::constants


namespace oura_charts::logging
{

   /// <summary>
   ///   Log an exception with source information. This overload expects the exception to have
   ///   formatter support and will be directly output that way.
   /// </summary>
   template<ExceptionDerived ExceptionType>
   void exception(const ExceptionType& e, std::source_location source_loc = std::source_location::current())
   {
      auto file_name = detail::fileNameFromPath(source_loc.file_name());
      error("In {}:{} - {}", file_name, source_loc.line(), source_loc.function_name(), e);
   }


   /// <summary>
   ///   Log an exception with source information. This overload for non-formatter-compatible types
   ///   will just use e.what() for the error message.
   /// </summary>
   template<FormattedException ExceptionType>
   void exception(const ExceptionType& e, std::source_location source_loc = std::source_location::current())
   {
      auto file_name = detail::fileNameFromPath(source_loc.file_name());
      error("In {}:{} - {}", file_name, source_loc.line(), e.what());
   }


   /// <summary>
   ///   Create a logging sink that outputs log messages to the console
   /// </summary>
   [[nodiscard]] sink_ptr_t makeConsoleSink(log_level::level_enum level = constants::CONFIG_DEFAULT_LOGLEVEL_CONSOLE,
                                            std::string_view pattern = constants::CONFIG_DEFAULT_LOG_PATTERN_CONSOLE);


   /// <summary>
   ///   Create a daily file logging sink with the specified level, path/name and log pattern.
   /// </summary>
   [[nodiscard]] sink_ptr_t makeDailyFileSink(log_level::level_enum level,
                                              fs::path log_folder,
                                              std::string_view log_filename_base,
                                              std::string_view pattern);


   /// <summary>
   ///   Overload of makeDailyFileSink() that has default values for convenience.
   /// </summary>
   inline [[nodiscard]] sink_ptr_t makeDailyFileSink(log_level::level_enum level = constants::CONFIG_DEFAULT_LOGLEVEL_DAILYFILE,
                                                     std::string_view log_folder = constants::CONFIG_DEFAULT_LOG_FOLDER,
                                                     std::string_view log_filename_base = constants::APP_NAME_NOSPACE,
                                                     std::string_view pattern = constants::CONFIG_DEFAULT_LOG_PATTERN_CONSOLE)
   {                                   
      return makeDailyFileSink(level, fs::path{ log_folder }, log_filename_base, pattern);
   }


   /// <summary>
   ///   Init a log sink that writes output to OutputDebugStringA() on Windows Debug builds,
   ///   or nullptr otherwise.
   /// </summary>
   /// <remarks>
   ///   Note that this function returns a nullptr object if you're not on Windows. It's safe to pass in a
   ///   sinks_init_list{} to our LogInit objects, but you should check the value before passing directly
   ///   to the spdlog logging api.
   /// </remarks>
   [[nodiscard]] sinks_init_list::value_type makeDebuggerSync();


   struct LogFactory
   {
      /// <summary>
      ///   creates a logger instance that is not registered with spdlog and can only be used through
      ///   the returned object.
      /// </summary>
      /// <remarks>
      ///   note that 'max_level' should be the least restrictive log level you want to use with any
      ///   sink, as this level will be used to filter messages before calling any sinks. The log level
      ///   filter on a sink is in addition to the max_level.
      /// </remarks>
      static [[nodiscard]] log_ptr_t makePrivate(std::string_view log_name, sinks_init_list sinks, log_level::level_enum max_level);


      /// <summary>
      ///   create a logger instance and register it with spdlog so it can be used/retrieved through the spdlog
      ///   api's. Loggers of this type will outlive the returned shared_ptr<> unless you deregister (drop) them.
      /// </summary>
      /// <remarks>
      ///   note that 'max_level' should be the least restrictive log level you want to use with any
      ///   sink, as this level will be used to filter messages before calling any sinks. The log level
      ///   filter on a sink is in addition to the max_level.
      /// </remarks>
      static log_ptr_t makeRegistered(std::string_view log_name, sinks_init_list sinks, log_level::level_enum max_level);


      /// <summary>
      ///   create new logger and register it as the system default/global lgoger, available through the free-
      ///   standing log functions. This logger will outlive the returned shared_ptr<> since it's also
      ///   registered as the defualt logger.
      /// </summary>
      /// <remarks>
      ///   note that 'max_level' should be the least restrictive log level you want to use with any
      ///   sink, as this level will be used to filter messages before calling any sinks. The log level
      ///   filter on a sink is in addition to the max_level.
      /// </remarks>
      static log_ptr_t makeDefault(sinks_init_list sinks = { makeConsoleSink(), makeDailyFileSink() }, log_level::level_enum max_level = level_enum::trace);
   };
   
} //  namespace oura_charts
