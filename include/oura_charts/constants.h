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

namespace oura_charts::constants
{
   static constexpr const char* REST_URL_BASE = "https://api.ouraring.com//v2/usercollection";
   static constexpr const char* REST_PATH_PERSONAL_INFO = "personal_info";
   static constexpr const char* REST_PATH_HEART_RATE = "heartrate";

   static constexpr const char* REST_HEADER_XCLIENT = "X-Client";
   static constexpr const char* REST_HEADER_XCLIENT_VALUE = "restc-cpp";

   static constexpr const char* REST_PARAM_AUTH_TOKEN_PREFIX = "Bearer ";
   static constexpr const char* REST_PARAM_START_DATETIME = "start_datetime";
   static constexpr const char* REST_PARAM_END_DATETIME = "end_datetime";

   static constexpr int REST_DEFAULT_DATA_LIMIT{ 1024 * 1024 }; // 1MB

   static constexpr int MAX_ENV_VAR_LENGTH = 1024;

   static constexpr const char* ERROR_CATEGORY_SUCCESS = "Success";
   static constexpr const char* ERROR_CATEGORY_GENERIC = "Generic Runtime Error";
   static constexpr const char* ERROR_CATEGORY_REST = "REST Error";
   static constexpr const char* ERROR_CATEGORY_JSON = "JSON Error";

} // namespace oura_charts::constants
