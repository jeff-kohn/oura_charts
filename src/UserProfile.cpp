//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// UserProfile.h
///
/// Defines the UserProfile class
/// 
/// Copyright (c) 2024 Jeff Kohn
/// 
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "oura_charts/UserProfile.h"
#include <glaze/glaze.hpp>

namespace oura_charts
{
   using namespace oura_charts::detail;
   
   UserProfile::UserProfile(std::string_view json)
   {
      auto pe = glz::read_json(static_cast<user_data&>(*this), json);
      if (pe)
         throw oura_exception{
                  static_cast<int64_t>(pe.ec),
                  glz::format_error(pe, json),
                  ErrorCategory::Parse };
   }

} // namespace oura_charts
