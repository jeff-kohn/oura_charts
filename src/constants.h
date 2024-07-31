#pragma once

#include "oura_charts/constants.h"

namespace oura_charts::constants
{
   inline constexpr const char* CONFIG_SECTION_REST = "REST";
   inline constexpr const char* CONFIG_VALUE_PAT_VAR = "OURA_PAT";

   inline constexpr const char* FMT_MSG_ERROR_NO_PAT = "No Personal Access Token configured. Update Preferences with a valid token, or set the {0} environment variable.";

   inline constexpr const char* PASSWORD_DISPLAY = "********************************";

   inline constexpr const char* MSG_RESET_TOKEN_PROMPT = "Are you sure you want to reset the stored Personal Access Token? You won't be able to retrieve data from Oura until you configure a valid token.";
   inline constexpr const char* TITLE_RESET_TOKEN_PROMPT = "Reset PAT";

} // namespace oura_charts
