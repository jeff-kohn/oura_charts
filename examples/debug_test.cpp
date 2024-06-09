#include <string>
#include <chrono>
#include <iostream>
#include <glaze/glaze.hpp>

static constexpr std::string_view json =  R"(
   {
      "message": "Hello",
      "seconds_duration": 5458
   }
)";

namespace chrono = std::chrono;

namespace glz::detail
{
   template <>
   struct from_json<chrono::seconds>
   {
      template <auto Opts>
      static void op(chrono::seconds& value, is_context auto&& ctx, auto&&... args)
      {
         int32_t sec_count{};
         read<json>::op<Opts>(sec_count, ctx, args...);
         if (glz::error_code::none == ctx.error)
            value = chrono::seconds{ sec_count };
      }
   };
} // namespace glz::detail


struct chrono_data
{
   std::string  message{};
   chrono::seconds seconds_duration;
};


int main(int, char** )
{
   auto data_result = glz::read_json<chrono_data>(json);
   if (data_result)
      std::cout << "Message '" << data_result->message << "' had duration of " << data_result->seconds_duration << std::endl;
   else
      std::cerr << "Error - " << glz::format_error(data_result.error(), json) << std::endl;
   return 0;
}
