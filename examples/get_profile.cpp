#include "oura_charts/UserProfile.h"
#include "oura_charts/RestAuth.h"
#include "oura_charts/helpers.h"
#include <fmt/format.h>
#include <cxxopts.hpp>
#include <restc-cpp/restc-cpp.h>
#include <restc-cpp/RequestBuilder.h>
#include <boost/fusion/adapted.hpp>

using namespace std;
using namespace restc_cpp;

// C++ structure that match the JSON entries received
// from http://jsonplaceholder.typicode.com/posts/{id}
struct Post
{
   int userId = 0;
   int id = 0;
   string title;
   string body;
};

// Since C++ does not (yet) offer reflection, we need to tell the library how
// to map json members to a type. We are doing this by declaring the
// structs/classes with BOOST_FUSION_ADAPT_STRUCT from the boost libraries.
// This allows us to convert the C++ classes to and from JSON.

BOOST_FUSION_ADAPT_STRUCT(
    Post,
    (int, userId)
    (int, id)
    (string, title)
    (string, body)
)

int main(int argc, char* argv[])
{
   try
   {
      using namespace oura_charts;
      using std::string;

      cxxopts::Options options(argv[0], "OuraCharts Get User Profile");
      options.add_options()
         ("t,token", "Personal Access Token for your Oura cloud account", cxxopts::value<string>()->default_value(""))
         ("h, help", "show help", cxxopts::value<bool>());

      auto params = options.parse(argc, argv);
      string pat = params["token"].as<string>();
      if (pat.empty())
         pat = helpers::getEnvironmentVariable("OURA_PAT");
      if (pat.empty())
         throw std::runtime_error("Personal Access Token not specified. Use the --token parameter or set the OURA_TOKEN environment variable.");

      if (params.count("help"))
      {
         fmt::println("{}", options.help());
         return 0;
      }


      // Create an instance of the rest client
      auto rest_client = RestClient::Create();

      // Create and instantiate a Post from data received from the server.
      Post my_post = rest_client->ProcessWithPromiseT<Post>([&] (Context& ctx)
      {
            // This is a co-routine, running in a worker-thread

            // Instantiate a Post structure.
            Post post;

            // Serialize it asynchronously. The asynchronously part does not really matter
            // here, but it may if you receive huge data structures.
            SerializeFromJson(post,

            // Construct a request to the server
            RequestBuilder(ctx)
            .Get("http://jsonplaceholder.typicode.com/posts/1")

            // Add some headers for good taste
            .Header("X-Client", "RESTC_CPP")
            .Header("X-Client-Purpose", "Testing")

            // Send the request
            .Execute());

            // Return the post instance trough a C++ future<>
            return post;
      })

      // Get the Post instance from the future<>, or any C++ exception thrown
      // within the lambda.
         .get();

     // Print the result for everyone to see.
      cout << "Received post# " << my_post.id << ", title: " << my_post.title << endl;

      oura_charts::TokenAuth auth{ pat };
      auto profile = oura_charts::UserProfile::getProfile(auth);

   }
   catch (std::exception& e)
   {
      fmt::println("Error: {}", e.what());
   }
}
