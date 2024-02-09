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

namespace oura_charts::constants::rest_urls
{

   static constexpr const char*  OURA_URL_PERSONAL_INFO = "https://api.ouraring.com/v2/usercollection/personal_info";

} //  oura_charts::rest_constants::urls

namespace oura_charts::constants::rest_headers
{
   
   static constexpr const char* OURA_REST_HEADER_AUTH = "Authorization";

} // namespace oura_cahrts::rest_constants::headers


namespace oura_charts::constants::rest_params
{

   static constexpr const char* OURA_REST_PARAM_AUTH_TOKEN_PREFIX = "Bearer ";

} // namespace oura_cahrts::rest_constants::headers
