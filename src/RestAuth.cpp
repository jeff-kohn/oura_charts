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
   cpr::Bearer TokenAuth::getAuthorization() const
   {
      return cpr::Bearer{ m_token };
   }

}
