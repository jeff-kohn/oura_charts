
#include <nlohmann/json.hpp>
#include <concepts>

namespace oura_charts::detail
{

   /// <summary>
   ///   concept requiring a template arge to be std::exception or derived from std::exception
   /// </summary>
   template<typename T>
   concept ExceptionDerived = std::derived_from<T, std::exception> || std::same_as<T, std::exception>;


   /// <summary>
   ///   concept requiring a type to be string_view or convertible to string_view
   /// </summary>
   template <typename T>
   concept StringViewCompatible = std::convertible_to<T, std::string_view>;


   /// <summary>
   ///   concept for a class that is can be constructed from JSON and knows the URL
   ///   endpoint to retrieve its data from.
   /// </summary>
   template<typename T>
   concept RestConstructable = requires (T t)
   {
      T::REST_PATH;
      T{ nlohmann::json{} };
   };

   /// <summary>
   ///   concept for a class that is can be constructed from JSON and knows the URL
   ///   endpoint to retrieve its data from.
   /// </summary>
   template<typename T>
   concept RestNoThrowConstructable = requires (T t, expected<T, oura_exception> retval)
   {
      T::REST_PATH;
      retval = T::makeFromJson(nlohmann::json{});
   };


   /// <summary>
   ///   concept for a class that can contain a sequence of rest-constructible
   ///   objects.
   /// </summary>
   template<typename Container>
   concept RestDataSet = std::default_initializable<Container>
      && std::ranges::range<Container>
      && RestNoThrowConstructable<typename Container::value_type::value_type>
      && requires (Container cont, Container::value_type val)
      {
         Container::value_type::value_type::REST_PATH;
         cont.push_back(val);
         cont.reserve(55);
      };

}
