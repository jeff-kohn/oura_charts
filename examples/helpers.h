#pragma once
#include "oura_charts/detail/utility.h"
#include <cxxopts.hpp>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

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

/// <summary>
///   just dump some text to a file.
/// </summary>
inline bool saveTextToFile(oura_charts::StringViewCompatible auto text, fs::path file_path, bool overwrite = true) noexcept
{
   if (fs::exists(file_path) && !overwrite)
      return false;

   if (file_path.has_parent_path())
      fs::create_directories(file_path.parent_path());

   std::ofstream file_out{ file_path };
   file_out << text;
   return true;
}
