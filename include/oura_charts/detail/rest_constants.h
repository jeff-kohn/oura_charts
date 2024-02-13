//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// rest_constants.h
///
/// Defines the various constants used with the REST API (urls, headers, param names, etc)
///
/// Copyright (c) 2024 Jeff Kohn
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace oura_charts::constants
{
   static constexpr const char* OURA_URL_PERSONAL_INFO = "https://api.ouraring.com/v2/usercollection/personal_info";

   static constexpr const char* OURA_REST_HEADER_AUTH = "Authorization";
   static constexpr const char* OURA_REST_HEADER_XCLIENT = "X-Client";
   static constexpr const char* OURA_REST_HEADER_XCLIENT_VALUE = "restc-cpp";

   static constexpr const char* OURA_REST_PARAM_AUTH_TOKEN_PREFIX = "Bearer ";

   static constexpr int OURA_REST_DEFAULT_DATA_LIMIT{ 1024 * 1024 }; // 1MB
   static constexpr int OURA_MAX_ENV_VAR_LENGTH = 1024;

} // namespace oura_charts::constants
