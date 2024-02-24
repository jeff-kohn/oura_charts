//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// helpers.h
///
/// miscellaneous utility functions/classes/wrappers used throughout the project.
///
/// Copyright (c) 2024 Jeff Kohn
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <string_view>

namespace oura_charts::detail
{
   
   /// <summary>
   ///   'overloaded' utility class. Allows you to inherit from multiple classes (or lambdas),
   ///   inheriting their operator() so that it's overloaded for multiple signatures in one class.
   /// </summary>
   template<class... Ts> struct overload : Ts...
   {
      using Ts::operator()...;
      //consteval void operator()(auto) const { static_assert(false, "Unsupported type");  }
   };


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
   ///   helper to format relative REST path as a complete URL
   /// </summary>
   inline cpr::Url pathToUrl(std::string_view relative_path)
   {
      return { fmt::format("{}/{}", constants::REST_URL_BASE, relative_path) };
   }


}
