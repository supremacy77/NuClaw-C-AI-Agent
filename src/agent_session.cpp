#include "agent_session.h"
#include <iostream>
#include <sstream>


void AgentSession::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(asio::buffer(buffer_),
        [this, self](const boost::system::error_code& ec, std::size_t length) {
            if (ec) {
                std::cerr << "Read error: " << ec.message() << std::endl;
                return;
            }

            Request req;
            if (http_parser_.parse(buffer_.data(), length, req)) {
                handle_chat(req);
            } else {
                do_read();
            }
        });
}


void AgentSession::handle_chat(const Request& req) {
    std::string message = req.get_query_param("message");
    auto result = rule_engine_.recognize(message);
    std::string reply;
    if (result && result->confidence > 0.5f) {
        auto* intent = rule_engine_.find_intent(result->intent);
        if (intent && intent->handler) {
            intent->handler(result->entities);
            reply = "Intent recognized: " + result->intent;
        } else {
            reply = "Intent recognized but no handler found.";
        }
    } else {
        reply = "I couldn't understand that.";
    }
    do_write(reply);
}


void AgentSession::do_write(const std::string& response) {
    auto self(shared_from_this());
    std::ostringstream response_stream;
    response_stream << "HTTP/1.1 200 OK\r\n"
                    << "Content-Length: " << response.size() << "\r\n"
                    << "Content-Type: text/plain\r\n"
                    << "Connection: close\r\n"
                    << "\r\n"
                    << response;

    std::string full_response = response_stream.str();
    asio::async_write(socket_, asio::buffer(full_response),
        [this, self](const boost::system::error_code& ec, std::size_t) {
            if (ec) {
                std::cerr << "Write error: " << ec.message() << std::endl;
                return;
            }
            socket_.close();
        });
}

