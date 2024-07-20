#pragma once

namespace oura_charts::constants
{
   inline constexpr const char* CONFIG_SECTION_REST = "REST";
   inline constexpr const char* CONFIG_VALUE_REST_PAT = "token";
   inline constexpr const char* CONFIG_VALUE_PAT_VAR = "OURA_PAT";

   inline constexpr const char* ERROR_MSG_FMT_NO_PAT = "No Personal Access Token configured. Update Preferences with a valid token, or set the {0} environment variable.";


} // namespace oura_charts