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
#include "oura_charts/detail/rest_constants.h"
#include "restc-cpp/restc-cpp.h"
#include "restc-cpp/RequestBuilder.h"

namespace oura_charts
{
   UserProfile UserProfile::getProfile(RestAuth& auth)
   {
      using oura_charts::detail::user_data,
         restc_cpp::RestClient,
         restc_cpp::RequestBuilder,
         restc_cpp::Context;

      // Create the REST clent and run the GET request asynchronously
      auto client = RestClient::Create();

      auto func = [&] (Context& ctx)
         {
            RequestBuilder req(ctx);
            req.Get(constants::OURA_URL_PERSONAL_INFO);
            req.Header(constants::OURA_REST_HEADER_XCLIENT,
                       constants::OURA_REST_HEADER_XCLIENT_VALUE);
            auth.setHeaders(req);

            detail::user_data data;
            auto reply = req.Execute();
            SerializeFromJson(data, std::move(reply));
            return data;
         };

      auto promise = client->ProcessWithPromiseT<detail::user_data>(func);
      user_data data{ promise.get() };
      return UserProfile(std::move(data));
   }

} // namespace oura_charts
