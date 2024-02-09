#pragma once

#include <string>
#include <expected>


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
   class user_profile : protected detail::user_data
   {
      public:
         user_profile() = delete;
         user_profile(const user_profile &) = default;
         user_profile(user_profile &&) = default;
         ~user_profile() = default;

         user_profile& operator=(const user_profile &) = default;
         user_profile& operator=(user_profile&&) = default;

         std::string id() const    { return m_data.id;        }
         std::string email() const { return m_data.email;     }
         int age() const           { return m_data.age;       }
         int weight_kg() const     { return m_data.weight_kg; }
         int height_cm() const     { return m_data.height_cm; }
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
         user_profile(user_data &&data) : m_data{ data } {}

         detail::user_data m_data;
   };

} // namespace oura_charts