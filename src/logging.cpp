//---------------------------------------------------------------------------------------------------------------------
// logging.cpp
//
// defines the logging namespace and interface for logging functionality.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#include "oura_charts/detail/logging.h"
#include "oura_charts/detail/utility.h"
#include <ranges>
#include <cassert>
#include <algorithm>

namespace oura_charts::logging
{
   namespace rgs = std::ranges;
   namespace vws = rgs::views;
   using namespace oura_charts::detail;
   using std::make_shared;
   using std::move;
   using std::string_view;
   using std::string;


   [[nodiscard]] sink_ptr_t makeConsoleSink(log_level::level_enum level, string_view pattern)
   {
      auto sink = make_shared<sinks::stdout_color_sink_mt>();
      sink->set_level(level);
      sink->set_pattern(string{ pattern });
      return sink;
   }


   [[nodiscard]] sink_ptr_t makeDailyFileSink(log_level::level_enum level,
                                              fs::path log_folder,
                                              string_view log_filename_base,
                                              string_view pattern)
   {
      auto log_path{ log_folder / log_filename_base };
      if (!log_path.has_extension())
         log_path.replace_extension(".log");

      std::string logfile{ log_path.generic_string() };
      expandEnvironmentVars(logfile);

      auto file_sink = make_shared<daily_file_sink_mt>(logfile, 0, 1);
      file_sink->set_level(level);
      file_sink->set_pattern(string{ pattern });
      return file_sink;
   }


   [[nodiscard]] sinks_init_list::value_type makeDebuggerSync()
   {
   #if !defined(NDEBUG) && defined(_WIN32)
      // log output to MSVC debug window.
      auto windbg_sink = make_shared<msvc_sink_mt>();
      windbg_sink->set_level(spdlog::level::trace);
      return move(windbg_sink);
   #else
      return {};
   #endif
   }


   [[nodiscard]] log_ptr_t LogFactory::makePrivate(string_view log_name,
                                                   sinks_init_list sinks,
                                                   log_level::level_enum max_level)
   {
      try
      {
         // Filter any nullptr's out, since debuggerSync may not always be available.
         auto sink_view = vws::filter(sinks, [] (sinks_init_list::value_type val) -> bool
                                    {
                                          return val.get() != nullptr;
                                    });

         auto logger = make_shared<log_ptr_t::element_type>(string{ log_name }, rgs::begin(sink_view), rgs::end(sink_view));
         logger->set_level(max_level);
         set_default_logger(logger);
         return logger;
      }
      catch (std::exception& e)
      {
         error("Error initializing logger {}: {}", log_name, e.what());
         throw;
      }
   }


   log_ptr_t LogFactory::makeRegistered(string_view log_name, sinks_init_list sinks, log_level::level_enum max_level)
   {
      auto logger = makePrivate(log_name, sinks, max_level);
      spdlog::drop(logger->name()); // name collision with existing logger will throw.
      spdlog::register_logger(logger);
      return logger;
   }


   log_ptr_t LogFactory::makeDefault(sinks_init_list sinks, log_level::level_enum max_level)
   {
      auto logger = makePrivate(constants::CONFIG_DEFAULT_LOG_NAME, sinks, max_level);
      spdlog::drop(logger->name()); // name collision with existing logger will throw.
      set_default_logger(logger);
      return logger;
   }


} // namespace oura_charts::logging
