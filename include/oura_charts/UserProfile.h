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
      int age{};
      int weight_kg{};
      int height_cm{};
      std::string biological_sex{};
      std::string email{};
   };
}


namespace oura_charts
{
   /// <summary>
   /// This class represents the User Profile data from the Oura Cloud API.
   /// </summary>
   class UserProfile : protected detail::user_data
   {
   public:
      UserProfile(user_data &&data) : m_data{ data } {}
      UserProfile() = delete;
      UserProfile(const UserProfile &) = default;
      UserProfile(UserProfile &&) = default;
      ~UserProfile() = default;

      UserProfile &operator=(const UserProfile &) = default;
      UserProfile &operator=(UserProfile &&) = default;

      std::string id() const { return m_data.id; }
      std::string email() const { return m_data.email; }
      int age() const { return m_data.age; }
      int weight_kg() const { return m_data.weight_kg; }
      int height_cm() const { return m_data.height_cm; }
      std::string biological_sex() const { return m_data.biological_sex; }

      /// <summary>
      /// retrieves the user profile for the specified personal access token.
      /// </summary>
      //template <typename Context>
      //static std::expected<user_profile, rest_error> get_profile(std::string_view token, Context &context)
      //{
      //   return user_profile{};
      //}

   private:
      detail::user_data m_data;
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
      else if constexpr (Idx == 3) { return user.weight_kg(); }
      else if constexpr (Idx == 4) { return user.height_cm(); }
      else if constexpr (Idx == 5) { return user.biological_sex(); }
      else { return user.biological_sex(); }
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




