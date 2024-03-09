//---------------------------------------------------------------------------------------------------------------------
// utlity.cpp
//
// defines miscellaneous utility functions/classes/wrappers used throughout the project.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

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
   using std::string;
   using std::string_view;


   string_view fileNameFromPath(string_view path) noexcept
   {
      auto sep = path.find_last_of('\\');
      if (string_view::npos == sep)
         sep = path.find_last_of("/");

      if (string_view::npos == sep)
         return path;
      else
         return path.substr(sep + 1);
   }


   string getEnvironmentVariable(const char* var_name, string_view default_val)
   {
      if (nullptr == var_name || *var_name == '\0')
         return string(default_val);

#if defined(_WIN32_WINNT)
      // std::getenv() is problematic on Windows so use the WinAPI
      char buf[constants::MAX_ENV_VAR_LENGTH] = {'\0'};
      if (::GetEnvironmentVariableA(var_name, buf, sizeof(buf) - 1))
         return string{ buf };
      else
         return string{ default_val };
#else
      const char* val_ptr = std::getenv(var_name);
      if (nullptr == val_ptr)
         return string{ default_val };

      string_view sv{ val_ptr };
      if (sv.length() > constants::MAX_ENV_VAR_LENGTH)
         return string{ sv.substr(0, constants::MAX_ENV_VAR_LENGTH) };
      else
         return string{ sv };
#endif
   }



#if defined(_WIN32)


   bool expandEnvironmentVars(string& text)
   {
      // Find out how big of a string we need to accomodate
      auto bufsize = ExpandEnvironmentStrings(text.c_str(), nullptr, 0);
      if (0 == bufsize)
         return false;

      // When using ANSI strings, the buffer size should be the string length,
      // plus terminating null character, plus one. 
      string dest(bufsize+2, '\0');

      if (ExpandEnvironmentStrings(text.c_str(), dest.data(), bufsize))
      {
         text.swap(dest);
         return true;
      }
      else
         return false;
   }


#else // Linux

   /// <summary>
   ///   Expand environment varariables in a string (in-place). Returns true if
   ///   successful, false if unsuccessful in which case text will be unchanged.
   /// </summary>
   bool expandEnvironmentVars(string& text)
   {
      using namespace std;
      static regex envRegex("\\$(\\w+|\\{\\w+\\})", regex::ECMAScript);

      // 0,1 indicates to get the full match + first subgroup
      size_t offset = 0;
      const string matchText = text;
      sregex_token_iterator matchIter(matchText.begin(), matchText.end(), envRegex, { 0, 1 });
      for (sregex_token_iterator end; matchIter != end; ++matchIter)
      {
         const string match = matchIter->str();
         string envVarName = (++matchIter)->str();

         // Remove matching braces
         if (envVarName.front() == '{' && envVarName.back() == '}')
         {
            envVarName.erase(envVarName.begin());
            envVarName.erase(envVarName.end() - 1);
         }

         // Search for env var and replace if found
         const char* s = getenv(envVarName.c_str());
         if (s != nullptr)
         {
            string value(s);

            // Handle nested env vars
            expandEnvironmentVars(value);

            // Since we're manipulating the string, do a new find
            // instead of using original match info
            size_t pos = text.find(match, offset);
            if (pos != string::npos)
            {
               text.replace(pos, match.length(), value);
               offset = pos + value.length();
            }
         }
         else
         {
            offset += match.length();
         }
      }
      return true;
   }
#endif


} // namespace oura_charts::detail
