#include "oura_charts/UserProfile.h"
#include "oura_charts/RestAuth.h"
#include <boost/fusion/adapted.hpp>
#include <restc-cpp/restc-cpp.h>
#include <restc-cpp/RequestBuilder.h>

// clang-format off

/// <summary>
/// This struct maps user profile data to the JSON response from
/// GET OURA_URL_PERSONAL_INFO
/// GET OURA_URL_PERSONAL_INFO
/// </summary>
BOOST_FUSION_ADAPT_STRUCT(oura_charts::detail::user_data,
   (std::string, id)
   (int, age)
   (int, weight_kg)
   (int, height_cm)
   (std::string, biological_sex)
   (std::string, email)
)


// clang-format off



 