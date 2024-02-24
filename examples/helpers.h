#pragma once
#include "oura_charts/detail/utility.h"

#include <cxxopts.hpp>
#include <string>


/// <summary>
///   try to get PAT from cmdline args or env variable. throws exception
///   if unsuccessful.
/// </summary>
inline std::string getPersonalToken(cxxopts::ParseResult& options)
{
   // PAT token either needs to be supplied on the command line or in an environment variable.
   auto pat = options["token"].as<std::string>();

   if (pat.empty())
      return oura_charts::detail::getEnvironmentVariable("OURA_PAT");
   else
      throw std::runtime_error("Personal Access Token not specified. Use the --token parameter or set the OURA_PAT environment variable.");
}
