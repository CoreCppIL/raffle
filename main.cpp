#include <random>
#include <iostream>
#include <filesystem>
#include "beast_https_get.hpp"
#include "Range-V3-VS2015/include/range/v3/all.hpp"
#include "json.hpp"

int main(int argc, char** argv) 
{
   using namespace ranges;
   using namespace std::string_literals;
   namespace fs = std::experimental::filesystem;

   using std::cout;
   using std::flush;
   using std::cin;
   using std::cerr;

   try
   {
      if (argc < 3)
         return cout << "Usage: " << fs::path(argv[0]).filename() << " <member-id> <API key>\n", EXIT_SUCCESS;

      std::string member_id = argv[1], api_key = argv[2];

      auto rest_call = "/CoreCpp/events/244537670/rsvps?photo-host=public&only=member.name%2Cresponse"s;
      auto body_json = https_get("api.meetup.com", rest_call + "&sig_id=" + member_id + "&sig=" + api_key);
      if (body_json.empty())
         throw std::runtime_error("MeetUp API REST call failed.");

      auto json = nlohmann::json::parse(body_json);
      if (json.find("errors") != json.end())
         throw std::runtime_error("REST call error in JSON:" + json["errors"].dump());

      std::mt19937 gen(std::random_device{}());
      auto rsvps = json                                                                // json is a valid range
         | view::remove_if([](auto&& elem) { return "yes" != elem["response"]; })      // filter out non-confirmed
         | view::transform([](auto&& elem) { return elem["member"]["name"].dump(); })  // keep name as string
         | ranges::to_vector                                                           // convert lazy range to vector - will be stored as rsvps
         | action::shuffle(gen);                                                       // random shuffle vector elements.

      for (auto&& name : rsvps)
      {
         cout << "> And the winner is: " << name << "!\n> " << flush;
         std::string more;
         cin >> more;
         if ("more" != more)
            break;
      }
   }
   catch (std::exception const& e) { std::cerr << "Error: " << e.what() << std::endl; }
   catch (...)                     { std::cerr << "Unknown Error!"      << std::endl; }
   return EXIT_FAILURE;
}