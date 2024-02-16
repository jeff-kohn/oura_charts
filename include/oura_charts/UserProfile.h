//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// UserProfile.h
///
/// Declares the UserProfile class and related details.
///
/// Copyright (c) 2024 Jeff Kohn
///
//////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "oura_charts/RestAuth.h"
#include "oura_charts/detail/rest_constants.h"
#include <cpr/cpr.h>
#include <string>
#include <utility>


namespace oura_charts::detail
{
   /// <summary>
   /// JSON struct for personal info profile from Oura
   /// </summary>
   struct user_data
   {
      std::string id;
      std::string email{};
      int age{};
      int weight_kg{};
      int height_m{};
      std::string biological_sex{};
   };
}

namespace oura_charts
{
   /// <summary>
   /// This class represents the User Profile data from the Oura Cloud API.
   /// </summary>
   /// <remarks>
   /// The only public ctors are for copy/move, to create instance of this
   /// class use the static factory method getProfile()
   /// </remarks>
   class UserProfile : protected detail::user_data
   {
   public:
      UserProfile(const UserProfile &) = default;
      UserProfile(UserProfile &&) = default;
      ~UserProfile() = default;

      UserProfile &operator=(const UserProfile &) = default;
      UserProfile &operator=(UserProfile &&) = default;

      std::string id() const     { return user_data::id;    }
      std::string email() const  { return user_data::email; }
      int age() const            { return user_data::age;   }

      /// Weight in kg
      int weight() const         { return user_data::weight_kg; }

      /// Height in meters
      int height() const         { return user_data::height_m; }

      std::string biologicalSex() const { return user_data::biological_sex; }

      template<typename AuthType>
      static UserProfile getProfile(const AuthWrapper<AuthType>& auth)
      {
         cpr::Session session{};
         session.SetOption(auth.getAuthorization() );
         session.SetOption(cpr::Header{ {constants::OURA_REST_HEADER_XCLIENT, constants::OURA_REST_HEADER_XCLIENT_VALUE} });
         session.SetOption(cpr::Url{ constants::OURA_REST_URL_PERSONAL_INFO} );

         auto response = session.Get();
         return UserProfile{};
      }

   private:
      UserProfile() = default;
      UserProfile(user_data&& data) : detail::user_data{ std::move(data) } {}
   };

   /// <summary>
   /// get overload for structured binding support for the UserProfile class.
   /// </summary>
   template < std::size_t Idx > auto get(const oura_charts::UserProfile &user)
   {
      static_assert(Idx < 6);

      if constexpr (Idx == 0) { return user.id(); }
      else if constexpr (Idx == 1) { return user.email(); }
      else if constexpr (Idx == 2) { return user.age(); }
      else if constexpr (Idx == 3) { return user.weight(); }
      else if constexpr (Idx == 4) { return user.height(); }
      else { return user.biologicalSex(); }
   }
}


/// <summary>
/// provide a tuple-like API for class UerProfile for structured bindings:
/// </summary>
template <> struct std::tuple_size <oura_charts::UserProfile> { static constexpr int value = 6; };
template <> struct std::tuple_element<0, oura_charts::UserProfile> { using type = std::string; };
template <> struct std::tuple_element<1, oura_charts::UserProfile> { using type = std::string; };
template <> struct std::tuple_element<2, oura_charts::UserProfile> { using type = int; };
template <> struct std::tuple_element<3, oura_charts::UserProfile> { using type = int; };
template <> struct std::tuple_element<4, oura_charts::UserProfile> { using type = int; };
template <> struct std::tuple_element<5, oura_charts::UserProfile> { using type = std::string; };




