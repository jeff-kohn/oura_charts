//---------------------------------------------------------------------------------------------------------------------
// constants.h
//
// header file for constants used throughout the client app
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/constants.h"

namespace oura_charts::constants
{
   inline constexpr const char* CONFIG_SECTION_REST = "REST";
   inline constexpr const char* CONFIG_VALUE_PAT_VAR = "OURA_PAT";

   inline constexpr const char* PASSWORD_DISPLAY = "********************************";

   inline constexpr const char* FMT_MSG_ERROR_NO_PAT = "No Personal Access Token configured. Update Preferences with a valid token, or set the {0} environment variable.";
   inline constexpr const char*     MSG_ERROR_NO_PAT = "Secret store unavailable. Linux users need to install and configure a keyring before credentials can be saved. In the meantime, you can use an environment variable if you wish.";

   inline constexpr const char* FMT_MSG_ERROR_SAVE_SECRET_FAILED = "An error occurred while updating the Personal Access Token.\r\n\r\n{}";
   inline constexpr const char*     MSG_ERROR_SAVE_SECRET_FAILED = "Unable to save token/credential to secret store. See logs for more information.";

   inline constexpr const char* MSG_ERROR_DELETE_SECRET_FAILED = "Unable to remove token/credential from Secret Store. See logs for more information.";

   inline constexpr const char*   MSG_RESET_TOKEN_PROMPT = "Are you sure you want to reset the stored Personal Access Token?\r\n\r\nYou won't be able to retrieve data from Oura until you configure a valid token.";
   inline constexpr const char* TITLE_RESET_TOKEN_PROMPT = "Reset PAT";


   inline constexpr const char* DLG_TITLE_ADD_DATA_FIELD = "Add Data Field";

   inline constexpr const char* APP_SCHEMA_FOLDER = "Schema";

} // namespace oura_charts
