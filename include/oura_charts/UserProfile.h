//---------------------------------------------------------------------------------------------------------------------
// UserProfile.h
//
// Declares the UserProfile class and related details.
//
// Copyright (c) 2024 Jeff Kohn. All Right Reserved.
//---------------------------------------------------------------------------------------------------------------------

#pragma once

#include "oura_charts/oura_charts.h"
#include "oura_charts/detail/utility.h"
#include "oura_charts/detail/json_structs.h"
#include <cpr/cpr.h>
#include <string>
#include <utility>


namespace oura_charts
{
   /// <summary>
   ///   This class represents the User Profile data from the Oura Cloud API.
   /// </summary>
   /// <remarks>
   ///   No default construction, aside from copy/move objects of this type
   ///   must be initialized by valid a JSON object.
   /// </remarks>
   class UserProfile
   {
   public:
      // path that will be used with base URL for REST call.
      static inline constexpr std::string_view REST_PATH = constants::REST_PATH_PERSONAL_INFO;

      std::string id() const     { return m_data.id;    }
      std::string email() const  { return m_data.email; }
      int age() const            { return m_data.age;   }

      /// Weight in kg
      double weight() const      { return m_data.weight; }

      /// Height in meters
      double height() const      { return m_data.height; }

      std::string biologicalSex() const { return m_data.biological_sex; }

      explicit UserProfile(detail::profile_data data) noexcept : m_data(std::move(data)) {}
      ~UserProfile() = default;
      UserProfile(const UserProfile&) = default;
      UserProfile(UserProfile&&) = default;
      UserProfile& operator=(const UserProfile&) = default;
      UserProfile& operator=(UserProfile&&) = default;

   private:
      detail::profile_data m_data;
   };


   /// <summary>
   ///   Retrieve a UserProfile object from the rest data provider.
   /// </summary>
   /// <remarks>
   ///   will thrown an exception if something goes wrong retrieving the profile.
   /// </remarks>
   template <DataProvider ProviderT>
   [[nodiscard]] UserProfile getUserProfile(const ProviderT& provider) noexcept(false)
   {
      auto json_res = provider.getJsonData(UserProfile::REST_PATH);
      if (!json_res)
         throw oura_exception{ json_res.error() };

      auto udt_res = detail::readJson<detail::profile_data>(json_res.value());
      if (!udt_res)
         throw oura_exception{ udt_res.error() };

      return UserProfile{ *udt_res };
   }


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

} // namespace oura_charts


/// <summary>
///   provide a tuple-like API for class UserProfile for structured bindings:
/// </summary>
template <> struct std::tuple_size <oura_charts::UserProfile> { static inline constexpr int value = 6; };
template <> struct std::tuple_element<0, oura_charts::UserProfile> { using type = std::string; };
template <> struct std::tuple_element<1, oura_charts::UserProfile> { using type = std::string; };
template <> struct std::tuple_element<2, oura_charts::UserProfile> { using type = int; };
template <> struct std::tuple_element<3, oura_charts::UserProfile> { using type = double; };
template <> struct std::tuple_element<4, oura_charts::UserProfile> { using type = double; };
template <> struct std::tuple_element<5, oura_charts::UserProfile> { using type = std::string; };

