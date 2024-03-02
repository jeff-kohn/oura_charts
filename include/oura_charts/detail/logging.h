//---------------------------------------------------------------------------------------------------------------------
// logging.h
//
// defines the log:: namespace and interface for logging.
// 
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/concepts.h"
#include "oura_charts/constants.h"
#include "oura_charts/oura_exception.h"
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <source_location>
#include <vector>
#include <memory>
#include <filesystem>

namespace oura_charts::log
{
   //
   // import these symbols into our namespace, so logging interface looks like log::warn(...) and there's
   // possiblity to replace logging backend with minimal impact if needed.
   //
   namespace fs = std::filesystem;
   namespace sinks = spdlog::sinks;
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


   // Logging factory that can be used for apps. TODO: get settings from config file.
   struct AppLogger
   {
      static log_ptr_t make(std::string_view log_folder = constants::CONFIG_DEFAULT_LOG_FOLDER,
                            std::string_view log_name = constants::CONFIG_DEFAULT_LOG_NAME)
      {
         auto console_sink = std::make_shared<sinks::stdout_color_sink_mt>();
         console_sink->set_level(spdlog::level::warn);
         console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [TID %t] [%l] %v");

         fs::path base_logfile = fs::path{ log_folder } / constants::APP_NAME_NOSPACE;
         auto file_sink = std::make_shared<daily_file_sink_mt>(base_logfile.string(), 0, 1);
         file_sink->set_level(spdlog::level::warn);
         file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%s:%#] [%^%l%$] %v");

         return  std::make_shared<spdlog::logger>(constants::CONFIG_DEFAULT_LOG_NAME,
                                                  sinks_init_list{ console_sink, file_sink });

      }
   };

   // logging factory for debugging
   struct DebugLogger
   {
      static log_ptr_t make(std::string_view log_name = constants::CONFIG_DEFAULT_LOG_NAME)
      {
         auto console_sink = std::make_shared<sinks::stdout_color_sink_mt>();
         console_sink->set_level(spdlog::level::info);
         console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [TID %t] [%l] %v");

         std::vector<spdlog::sink_ptr> sinks{ console_sink };

      #if !defined(NDEBUG) && defined(_WIN32)
	      // log output to MSVC debug window.
	      auto windbg_sink = std::make_shared<msvc_sink_mt>();
	      windbg_sink->set_level(spdlog::level::trace);
	      windbg_sink->set_pattern("****[%l]**** %v");
         sinks.push_back(std::move(windbg_sink));
      #endif

         return  std::make_shared<spdlog::logger>(std::string{ log_name }, std::begin(sinks), std::end(sinks));
      }
   };

   // logging factory for unit tests.
   struct TestLogger
   {
      static log_ptr_t make(std::string_view log_name = constants::CONFIG_DEFAULT_LOG_NAME )
      {
         auto console_sink = std::make_shared<sinks::stdout_color_sink_mt>();
         console_sink->set_level(spdlog::level::warn);
         console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [TID %t] [%l] %v");
         std::vector<spdlog::sink_ptr> sinks{ console_sink };

      #if !defined(NDEBUG) && defined(_WIN32)
         // log output to MSVC debug window.
         auto windbg_sink = std::make_shared<msvc_sink_mt>();
         windbg_sink->set_level(spdlog::level::trace);
         windbg_sink->set_pattern("****[%l]**** %v");
         sinks.push_back(windbg_sink);
      #endif

         return  std::make_shared<spdlog::logger>(std::string{ log_name }, std::begin(sinks), std::end(sinks));
      }
   };


   struct LogInit
   {
      LogInit(log_ptr_t&& log_ptr, bool make_default = true) : logger(std::move(log_ptr))
      {
         if (make_default)
            set_default_logger(logger);
      }

      log_ptr_t logger{};
   };
   
} //  namespace oura_charts
