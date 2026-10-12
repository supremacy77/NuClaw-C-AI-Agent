#pragma once
#include <boost/asio.hpp>
namespace asio = boost::asio;
using tcp = asio::ip::tcp;
class Router;

class Server {
public:
    Server(asio::io_context& io_context, short port, Router& router) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), router_(router) {
        do_accept();    
    };
private:
    void do_accept();
    tcp::acceptor acceptor_;
    Router& router_;
};