#include "server.h"
#include "session.h"

void Server::do_accept() {
    acceptor_.async_accept(
        [this](const boost::system::error_code& ec, tcp::socket socket) {
            if (!ec) {
                std::cout << "New connection accepted." << std::endl;
                std::make_shared<Session>(std::move(socket), router_)->start(); 
            } else {
                std::cerr << "Accept error: " << ec.message() << std::endl;
            }
            do_accept();
        });
}