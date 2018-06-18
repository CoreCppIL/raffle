#include <random>
#include <iostream>
#include <filesystem>
#include "beast_https_get.hpp"
#include "Range-V3-VS2015/include/range/v3/all.hpp"

using namespace ranges;
using namespace std::string_literals;
namespace fs = std::filesystem;

int main(int argc, char** argv) try
{
   if (argc < 3)
      return std::cout << "Usage: " << fs::path(argv[0]).filename() << " <API key> <Group Name>\n", EXIT_SUCCESS;
   auto api_key = argv[1];
   auto fetchGroup = [api_key](char const* urlname)
   {
      std::cout << "Welcome to the '" << urlname << "' Meeting Raffle!\nFetching Meetup details." << std::endl;
      auto meetup_rest_api_postfix = "&photo-host=public&sign=true&key="s + api_key;
      auto events_json = https_get_json("api.meetup.com", "/"s + urlname + "/events?" + meetup_rest_api_postfix);
      std::cout << "Fetching RSVP names for " << events_json.at(0)["name"] << std::endl; // assume first event is the current one
      std::string event_id = events_json.at(0)["id"];
      auto rsvp_json = https_get_json("api.meetup.com", "/"s + urlname + "/events/" + event_id + "/rsvps?" + meetup_rest_api_postfix);
      if (rsvp_json.find("errors") != rsvp_json.end())
         throw std::runtime_error("REST call error in JSON:" + rsvp_json["errors"].dump());
      return rsvp_json;
   };
   auto rsvps = view::delimit(argv + 2, nullptr)                                       // iterate on all group names from cli
      | view::transform(fetchGroup)                                                    // fetch JSON response for each group
      | view::transform([](auto&& json)                                                // filter JSOn to return only names who RSVPed yes
      { 
         return json
         | view::remove_if([](auto&& elem) { return "yes" != elem.at("response");  })  // filter out non-"yes" RSVP responses
         | view::transform([](auto&& elem) { return elem["member"]["name"].dump(); })  // keep name as string
         | ranges::to_vector;                                                          // convert lazy range to vector - will be stored as rsvps
      })
      | action::join                                                                   // merge (flatten) all names
      | action::sort                                                                   // sort elements.
      | action::unique                                                                 // remove duplicates
      | action::shuffle(std::mt19937{ std::random_device{}() });                       // random shuffle vector elements.

   for (auto&& name : rsvps)
   {
      std::cout << "And the winner is: " << name << "!\n> " << std::flush;
      std::string more;
      std::cin >> more;
      if ("more" != more)
         return EXIT_SUCCESS;
   }
}
catch (std::exception const& e) { std::cerr << "Error: " << e.what() << std::endl; }
catch (...)                     { std::cerr << "Unknown Error!"      << std::endl; }
