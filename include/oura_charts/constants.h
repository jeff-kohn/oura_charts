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
   static constexpr const char* OURACHARTS_REST_URL_PERSONAL_INFO = "https://api.ouraring.com/v2/usercollection/personal_info";

   static constexpr const char* OURACHARTS_REST_HEADER_XCLIENT = "X-Client";
   static constexpr const char* OURACHARTS_REST_HEADER_XCLIENT_VALUE = "restc-cpp";

   static constexpr const char* OURACHARTS_REST_PARAM_AUTH_TOKEN_PREFIX = "Bearer ";

   static constexpr int OURACHARTS_REST_DEFAULT_DATA_LIMIT{ 1024 * 1024 }; // 1MB

   static constexpr int OURACHARTS_MAX_ENV_VAR_LENGTH = 1024;

   static constexpr const char* OURACHARTS_ERROR_CATEGORY_SUCCESS = "Success";
   static constexpr const char* OURACHARTS_ERROR_CATEGORY_GENERIC = "Generic Runtime Error";
   static constexpr const char* OURACHARTS_ERROR_CATEGORY_REST = "REST Error";
   static constexpr const char* OURACHARTS_ERROR_CATEGORY_JSON = "JSON Error";

} // namespace oura_charts::constants
