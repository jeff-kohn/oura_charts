//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// RestAuth.cpp
///
/// Provides implmementation for the auth classes used to access the Oura REST API.
/// 
/// Copyright (c) 2024 Jeff Kohn
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////


#include "oura_charts/RestAuth.h"
#include "oura_charts/detail/rest_constants.h"
#include <fmt/format.h>

namespace oura_charts
{

   void TokenAuth::setHeaders(restc_cpp::RequestBuilder &req) 
   {
      req.Header(constants::rest_headers::OURA_REST_HEADER_AUTH,
                 fmt::format("{}{}", constants::rest_params::OURA_REST_PARAM_AUTH_TOKEN_PREFIX, m_token));
   }

}