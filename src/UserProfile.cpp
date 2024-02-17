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


namespace oura_charts::detail
{
   NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(user_data, id, email, age, weight, height, biological_sex)
}

namespace oura_charts
{
   using namespace oura_charts::detail;
   
   UserProfile::UserProfile(const detail::json& json_data)
   {
      try
      {
         json_data.get_to(static_cast<user_data&>(*this));
      }
      catch (json::exception& e)
      {
         throw translateException(e, this); 
      }
   }





} // namespace oura_charts
