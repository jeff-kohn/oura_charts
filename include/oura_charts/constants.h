//---------------------------------------------------------------------------------------------------------------------
// constants.h
//
// Defines various constants used throughout the project.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once


namespace oura_charts::constants
{
   inline constexpr const char* APP_NAME = "Oura Charts";
   inline constexpr const char* APP_NAME_NOSPACE = "Oura_Charts";

   inline constexpr const char* REST_DEFAULT_BASE_URL = "https://api.ouraring.com/v2/usercollection";
   inline constexpr const char* REST_PATH_PERSONAL_INFO = "personal_info";
   inline constexpr const char* REST_PATH_HEART_RATE = "heartrate";
   inline constexpr const char* REST_PATH_SLEEP_SESSION = "sleep";
   inline constexpr const char* REST_PATH_DAILY_SLEEP = "daily_sleep";

   inline constexpr const char* REST_HEADER_XCLIENT = "X-Client";
   inline constexpr const char* REST_HEADER_XCLIENT_VALUE = "cpr";

   inline constexpr const char* REST_PARAM_AUTH_TOKEN_PREFIX = "Bearer ";
   inline constexpr const char* REST_PARAM_START_DATETIME = "start_datetime";
   inline constexpr const char* REST_PARAM_END_DATETIME = "end_datetime";
   inline constexpr const char* REST_PARAM_START_DATE = "start_date";
   inline constexpr const char* REST_PARAM_END_DATE = "end_date";
   inline constexpr const char* REST_PARAM_NEXT_TOKEN = "next_token";

   inline constexpr const char* JSON_KEY_DATA = "data";

   inline constexpr int MAX_ENV_VAR_LENGTH = 1024;

   inline constexpr const char* UNIT_TEST_DATA_DIR = "./test_data";

   // string names for ErrorCategory enum values.
   inline constexpr const char* ERROR_CATEGORY_SUCCESS = "Success";
   inline constexpr const char* ERROR_CATEGORY_GENERIC = "Generic Runtime Error";
   inline constexpr const char* ERROR_CATEGORY_REST = "REST Error";
   inline constexpr const char* ERROR_CATEGORY_PARSE = "Parsing Error";
   inline constexpr const char* ERROR_CATEGORY_FILE_IO = "File I/O Error";
   inline constexpr const char* ERROR_CATEGORY_QUERY = "Query error";
   inline constexpr const char* ERROR_CATEGORY_SCHEMA = "Schema error";
   inline constexpr const char* ERROR_CONTEXT_REST_RESPONSE = "REST Response";

   // format strings and messages for error conditions
   inline constexpr const char* FMT_MSG_ERROR_SCHEMA_CLASS_NOT_FOUND = "The schema class '{}' was not found";


   // schema related constants
   inline constexpr const char* SCHEMA_FILE_SUFFIX = "schema.json";

   // these are used for parsing date/time values from string using
   // from_stream()
   inline constexpr const char* UTC_TIMEZONE = "Z";
   inline constexpr const char* PARSE_FMT_STR_ISO_DATETIME_LOCAL = "%FT%T%Ez";
   inline constexpr const char* PARSE_FMT_STR_ISO_DATETIME_UTC = "%FT%T%Z";
   inline constexpr const char* PARSE_FMT_STR_ISO_DATE_ONLY = "%F";


// Platform specific constants
#if defined(WIN32) || defined (_WIN32)

   inline constexpr const char* CONFIG_DEFAULT_LOG_FOLDER = "%LOCALAPPDATA%/oura_charts/logs";

#elif defined(__linux__)

   inline constexpr const char* CONFIG_DEFAULT_LOG_FOLDER = "$HOME/.local/state/oura_charts/logs";

#endif

} // namespace oura_charts::constants
