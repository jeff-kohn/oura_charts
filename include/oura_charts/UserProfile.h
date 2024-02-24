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

#include "oura_charts/oura_charts.h"
#include "oura_charts/RestAuth.h"
#include "oura_charts/detail/rest_helpers.h"
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
      double weight{};
      double height{};
      std::string biological_sex{};
   };
}

namespace oura_charts
{

   /// <summary>
   ///   This class represents the User Profile data from the Oura Cloud API.
   /// </summary>
   /// <remarks>
   ///   No default construction, aside from copy/move objects of this type
   ///   must be initialized by valid a JSON object.
   /// </remarks>
   class UserProfile : private detail::user_data
   {
   public:
      // URL used to retrieve 
      static constexpr std::string_view REST_PATH = constants::REST_PATH_PERSONAL_INFO;

      UserProfile(const detail::json& json_data);
      UserProfile(const UserProfile &) = default;
      UserProfile(UserProfile &&) = default;
      ~UserProfile() = default;
      UserProfile& operator=(const UserProfile&) = default;
      UserProfile& operator=(UserProfile&&) = default;

      /// <summary>
      ///   static factory method for getting a UserProfile object
      ///   from the REST API. Will throw oura_exception() if
      ///   unable to retrieve the data or initialize the object with it
      /// </summary>
      template<typename AuthType>
      static UserProfile getProfile(const AuthWrapper<AuthType>& auth)
      {
         return detail::getObjectFromRestEndpoint<UserProfile>(auth.getAuthorization());
      }

      std::string id() const     { return user_data::id;    }
      std::string email() const  { return user_data::email; }
      int age() const            { return user_data::age;   }

      /// Weight in kg
      double weight() const      { return user_data::weight; }

      /// Height in meters
      double height() const      { return user_data::height; }

      std::string biologicalSex() const { return user_data::biological_sex; }

   private:
      UserProfile() = default;
   };

   /// <summary>
   ///   get() overload for structured binding support for the UserProfile class.
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

   /// <summary>
   ///   custom format() support for UserProfile
   /// </summary>
   inline auto format_as(const UserProfile& profile)
   {
      return fmt::format("UserProfile for {} ({}, {}, id={})",
                         profile.email(),
                         profile.age(),
                         profile.biologicalSex(),
                         profile.id());
   }

}


/// <summary>
///   provide a tuple-like API for class UerProfile for structured bindings:
/// </summary>
template <> struct std::tuple_size <oura_charts::UserProfile> { static constexpr int value = 6; };
template <> struct std::tuple_element<0, oura_charts::UserProfile> { using type = std::string; };
template <> struct std::tuple_element<1, oura_charts::UserProfile> { using type = std::string; };
template <> struct std::tuple_element<2, oura_charts::UserProfile> { using type = int; };
template <> struct std::tuple_element<3, oura_charts::UserProfile> { using type = double; };
template <> struct std::tuple_element<4, oura_charts::UserProfile> { using type = double; };
template <> struct std::tuple_element<5, oura_charts::UserProfile> { using type = std::string; };

