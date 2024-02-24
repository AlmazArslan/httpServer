#include "server.h"

namespace ownHTTPServer
{
    http_connection::http_connection(tcp::socket socket, std::shared_ptr<Collector::FileCollector> fc)
        : socket_(std::move(socket)),
        filecollector(fc)
    {}

    void http_connection::start(void)
    {
        read_request();
    }

    void http_connection::read_request(void)
    {
        auto self = shared_from_this();

        http::async_read(
            socket_,
            buffer_,
            request_,
            [self](beast::error_code ec,
                std::size_t bytes_transferred)
            {
                boost::ignore_unused(bytes_transferred);
                if(!ec)
                    self->process_request();
            }
        );
    }

    void http_connection::process_request(void)
    {
        response_.version(request_.version());
        response_.keep_alive(false);

        switch (request_.method())
        {
            case (http::verb::get):
                response_.result(http::status::ok);
                response_.set(http::field::server, "Beast");
                create_response();
                break;
            default:
                response_.result(http::status::bad_request);
                response_.set(http::field::content_type, "text/plain");
                beast::ostream(response_.body())
                    << "Invalid request-method '"
                    << std::string(request_.method_string())
                    << "'";
                break;
        }
        write_response();
    }

    void http_connection::create_response(void)
    {
        std::string_view sv = request_.target().substr(1);
        std::string targetfile = {sv.data(), sv.size()};
        
        beast::ostream(response_.body())
            << filecollector->get_file_data(targetfile);
    }

    void http_connection::write_response(void)
    {
        auto self = shared_from_this();
        response_.content_length(response_.body().size());
        http::async_write(
            socket_,
            response_,
            [self](beast::error_code er, std::size_t bytes_trans)
            {
                self->socket_.shutdown(tcp::socket::shutdown_send, er);
            }
        );
    }
}

void ownHTTPServer::start_acceptor(tcp::acceptor& accepter, tcp::socket& sock,
    std::shared_ptr<Collector::FileCollector> fc)
{
    accepter.async_accept(sock,
        [&, fc](beast::error_code er){
            if (!er)
            {
                std::make_shared<ownHTTPServer::http_connection>
                    (std::move(sock), fc)->start();
            }
            start_acceptor(accepter, sock, fc); 
        });
}