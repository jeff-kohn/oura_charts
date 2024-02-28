#pragma once

#include <spdlog/spdlog.h>

namespace oura_charts
{
   // this is just an abstraction over the logging library we use under the hood. It also has some callbacks that
   // can be used for logging REST callls, etc.
   class Logger
   {
   public:
      using LogLevel = spdlog::level::level_enum;;
   }; 



} //  namespace oura_charts
