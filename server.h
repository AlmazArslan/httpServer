#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <cstdlib>
#include <memory>
#include <string>
#include <iostream>

#include "collector.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace ownHTTPServer
{
    class http_connection : public std::enable_shared_from_this<http_connection>
    {
    public:
        http_connection(tcp::socket socket, std::shared_ptr<Collector::FileCollector> fc);
        void start(void);
    private:
        std::shared_ptr<Collector::FileCollector> filecollector;
        tcp::socket socket_;
        beast::flat_buffer buffer_{8192};
        http::request<http::dynamic_body> request_;
        http::response<http::dynamic_body> response_;

        void read_request(void);
        void process_request(void);
        void write_response(void);
        void create_response(void);

        void read_handler(
            const boost::system::error_code& error,
            std::size_t bt
        );
    };

}

void start_acceptor(tcp::acceptor& a, tcp::socket& s, std::shared_ptr<Collector::FileCollector> fc);

#endif