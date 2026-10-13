#pragma once
#include <boost/asio.hpp>
namespace asio = boost::asio;
using tcp = asio::ip::tcp;
class RuleEngine;


class Server {
public:
    Server(asio::io_context& io_context, short port, RuleEngine& rule_engine)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), rule_engine_(rule_engine) {
        do_accept();
    }
private:
    void do_accept();
    tcp::acceptor acceptor_;
    RuleEngine& rule_engine_;
};