#include "server.h"
#include "agent_session.h"
#include <iostream>

void Server::do_accept() {
    acceptor_.async_accept(
        [this](const boost::system::error_code& ec, tcp::socket socket) {
            if (!ec) {
                std::cout << "New connection accepted." << std::endl;
                auto session = std::make_shared<AgentSession>(std::move(socket), rule_engine_);
                session->start();
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
            do_accept();
        });
}