#include "libs/beast/example/common/root_certificates.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <iostream>
#include <string>

#include "json.hpp"

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

nlohmann::json https_get_json(std::string const& host, std::string const& target)
{
   auto const port = "443";

   // The io_service is required for all I/O
   boost::asio::io_service ios;

   // The SSL context is required, and holds certificates
   ssl::context ctx{ ssl::context::sslv23_client };

   // This holds the root certificate used for verification
   load_root_certificates(ctx);

   // These objects perform our I/O
   tcp::resolver resolver{ ios };
   ssl::stream<tcp::socket> stream{ ios, ctx };

   // Look up the domain name
   auto const lookup = resolver.resolve({ host, port });

   // Make the connection on the IP address we get from a lookup
   boost::asio::connect(stream.next_layer(), lookup);

   // Perform the SSL handshake
   stream.handshake(ssl::stream_base::client);

   // Set up an HTTP GET request message
   http::request<http::string_body> req{ http::verb::get, target, 11 };
   req.set(http::field::host, host);
   req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

   // Send the HTTP request to the remote host
   http::write(stream, req);

   // This buffer is used for reading and must be persisted
   boost::beast::flat_buffer buffer;

   // Declare a container to hold the response
   http::response<http::string_body> res;

   // Receive the HTTP response
   http::read(stream, buffer, res);

   // Gracefully close the stream
   boost::system::error_code ec;
   stream.shutdown(ec);
   if (ec == boost::asio::error::eof)
   {
      // Rationale:
      // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
      ec.assign(0, ec.category());
   }
   if (ec)
      throw boost::system::system_error{ ec };

   // If we get here then the connection is closed gracefully
   return nlohmann::json::parse(res.body());
}



