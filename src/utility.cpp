#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/utility.h"

#if defined(_WIN32_WINNT)
   #include <windows.h>
   #include <vector> 
#else
   #include <cstdlib>
#endif

namespace oura_charts::detail
{

   std::string getEnvironmentVariable(const char* var_name, std::string_view default_val)
   {
      if (nullptr == var_name || *var_name == '\0')
         return std::string(default_val);

#if defined(_WIN32_WINNT)
      // std::getenv() is problematic on Windows so use the WinAPI
      char buf[constants::MAX_ENV_VAR_LENGTH] = {'\0'};
      if (::GetEnvironmentVariableA(var_name, buf, sizeof(buf) - 1))
         return std::string{ buf };
      else
         return std::string{ default_val };
#else
      const char* val_ptr = std::getenv(var_name);
      if (nullptr == val_ptr)
         return std::string{ default_val };

      std::string_view sv{ val_ptr };
      if (sv.length() > constants::MAX_ENV_VAR_LENGTH)
         return std::string{ sv.substr(0, constants::MAX_ENV_VAR_LENGTH) };
      else
         return std::string{ sv };
#endif
   }


} // namespace oura_charts::detail
