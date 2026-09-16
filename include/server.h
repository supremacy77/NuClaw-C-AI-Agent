#pragma once
#include <boost/asio.hpp>
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Server {
public:
    Server(asio::io_context& io_context, short port) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();    
    };
private:
    void do_accept();
    tcp::acceptor acceptor_;
};