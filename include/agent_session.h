#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <array>
#include "request.h"
#include "httpparser.h"
#include "rule_engine.h"

namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;
using boost::system::error_code;

class AgentSession : public std::enable_shared_from_this<AgentSession> {
public:
    AgentSession(tcp::socket socket, RuleEngine& rule_engine)
        : socket_(std::move(socket)), rule_engine_(rule_engine) {}
    void start() {
        do_read();
    };

private:
    void do_read();
    void handle_chat(const Request& req);
    void do_write(const std::string& response);
    tcp::socket socket_;
    RuleEngine& rule_engine_;
    HttpParser http_parser_;
    std::array<char, 8192> buffer_;
};