#include <boost/asio.hpp>
#include <memory>
#include <iostream>
#include "httpparser.h"
#include "request.h"

class Router;
class HttpParser;

namespace asio = boost::asio;
using tcp = asio::ip::tcp;


class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket, Router& router) : socket_(std::move(socket)), router_(router) {}

    void start() {
        do_read();
    }

private:
    void do_read();

    void do_write();

    void on_write(const boost::system::error_code& ec, std::size_t bytes_transferred);

    void handle_request();

    tcp::socket socket_;
    char buffer_[1024];
    Router& router_;
    HttpParser parser_;
    Request request_;
    std::string response_;
};
