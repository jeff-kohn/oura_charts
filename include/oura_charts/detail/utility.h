//---------------------------------------------------------------------------------------------------------------------
// utlity.h
//
// declares miscellaneous utility functions/classes/wrappers used throughout the project.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once
#include <string>
#include <string_view>


namespace oura_charts::detail
{

   /// <summary>
   ///   retrieve an environment variable. If the environment variable is not found, or an
   ///   error occurs, default_val will be returned. Otherwise the variable's value
   ///   (which could be an empty string) will be returned. Max value length returned
   ///   is MAX_ENV_VAR_LENGTH
   /// </summary>
   std::string getEnvironmentVariable(const char* var_name, std::string_view default_val = "");

   inline std::string getEnvironmentVariable(const std::string& var_name, std::string_view default_val = "")
   {
      return getEnvironmentVariable(var_name.c_str(), default_val);
   }


   /// <summary>
   ///   Expand environment varariables in a string (in-place). Returns true if
   ///   successful, false if unsuccessful in which case text will be unchanged.
   /// </summary>
   bool expandEnvironmentVars(std::string& text);


   /// <summary>
   ///   Get just the filename from a path. The returned value is a view based on the
   ///   passed-in path, so it is only valid for the lifetime of the passed-in path
   /// </summary>
   /// <remarks>
   ///   This might seem unnecessary but filesystem::path::filename does a bunch of
   ///   extra copying and converting between wstring and string on Windows, and even
   ///   on linux you're going to get 1-2 extra copies. This function does no copying
   /// <remarks>
   std::string_view fileNameFromPath(std::string_view path) noexcept;

} // namespace oura_charts::detail
