//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// constants.h
///
/// Defines the various constants used throughout the project.
///
/// Copyright (c) 2024 Jeff Kohn
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//#include <fmt/compile.h>

namespace oura_charts::constants
{
   static constexpr const char* REST_URL_BASE = "https://api.ouraring.com/v2/usercollection";
   static constexpr const char* REST_PATH_PERSONAL_INFO = "personal_info";
   static constexpr const char* REST_PATH_HEART_RATE = "heartrate";

   static constexpr const char* REST_HEADER_XCLIENT = "X-Client";
   static constexpr const char* REST_HEADER_XCLIENT_VALUE = "cpr";

   static constexpr const char* REST_PARAM_AUTH_TOKEN_PREFIX = "Bearer ";
   static constexpr const char* REST_PARAM_START_DATETIME = "start_datetime";
   static constexpr const char* REST_PARAM_END_DATETIME = "end_datetime";
   static constexpr const char* REST_PARAM_NEXT_TOKEN = "next_token";

   static constexpr const char* JSON_KEY_DATA = "data";

   static constexpr int REST_DEFAULT_DATA_LIMIT{ 1024 * 1024 }; // 1MB

   static constexpr int MAX_ENV_VAR_LENGTH = 1024;

   // string names for ErrorCategory enum values.
   static constexpr const char* ERROR_CATEGORY_SUCCESS = "Success";
   static constexpr const char* ERROR_CATEGORY_GENERIC = "Generic Runtime Error";
   static constexpr const char* ERROR_CATEGORY_REST = "REST Error";
   static constexpr const char* ERROR_CATEGORY_PARSE = "Parsing Error";
   static constexpr const char* ERROR_CONTEXT_REST_RESPONSE = "REST Response";

   // these are used for parsing date/time values from string using
   // from_stream()
   static constexpr const char* UTC_TIMEZONE = "Z";
   static constexpr const char* PARSE_FMT_STR_ISO_DATETIME_LOCAL = "%FT%T%Ez";
   static constexpr const char* PARSE_FMT_STR_ISO_DATETIME_UTC = "%FT%T%Z";

   // These need to be used at compile time with format(), and declaring them
   // as objects allows us to pass them as parameters, which we couldn't do
   // if they were just const char*
   //static constexpr fmt::format_string<detail::chrono::time_point> FMT_STR_DATETIME_ISO_UTC = "{:%FT%TZ}";
   //static constexpr auto FMT_STR_DATETIME_LOCAL_SHORT =  "{:%Om/%d/%Y %H}"_cf;

} // namespace oura_charts::constants
