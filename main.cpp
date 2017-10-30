#include <random>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

#include "beast_https_get.hpp"

#include "json.hpp"

using std::cout;
using std::flush;
using std::cin;
using std::cerr;

int main(int argc, char** argv)
{
   std::string host = "api.meetup.com";
   std::string target = "/CoreCpp/events/244537670/rsvps?photo-host=public&only=member.name%2Cresponse";
   std::string sig_id, key;
   if (argc == 3 && argv[1] && argv[2])
   {
      sig_id = argv[1];
      key = argv[2];
   }

   target += "&sig_id=" + sig_id;
   target += "&sig=" + key;

   auto body_json = https_get(host, target);

   if (body_json.empty())
   {
      cerr << "MeetUp API REST call failed :-(\n";
      return EXIT_FAILURE;
   }

   auto json = nlohmann::json::parse(body_json);

   if (json.find("errors") != json.end())
   {
      cerr << "REST call error :-(\n";
      return EXIT_FAILURE;
   }

   std::vector<std::string> rsvps;
   for (auto&& e:json)
      if ("yes" == e["response"])
         rsvps.push_back(e["member"]["name"].dump());

   std::random_device rd;
   std::mt19937 g(rd());
   std::shuffle(rsvps.begin(), rsvps.end(), g);
   
   while (0 < rsvps.size())
   {
      cout << "And the winner is: " << rsvps.back() << "\n\n";
      std::string more;

      cout << "Pick another? " << flush;
      cin >> more;
      if ("yes" != more)
         break;
      rsvps.pop_back();
   }        

   return EXIT_SUCCESS;
}
