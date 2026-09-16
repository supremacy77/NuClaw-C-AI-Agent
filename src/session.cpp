#include "session.h"

void Session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(asio::buffer(buffer_),
        [this, self](const boost::system::error_code& ec, std::size_t length) {
            on_read(ec, length);
        });
}

void Session::on_read(const boost::system::error_code& ec, std::size_t length) {
    if (ec) {
        if (ec == asio::error::eof) {
            std::cout << "Connection closed by client." << std::endl;
        } else {
            std::cerr << "Read error: " << ec.message() << std::endl;
        }
        return;
    }

    std::cout << "Received: " << std::string(buffer_, length) << std::endl;
    do_write(length);
}

void Session::do_write(std::size_t length) {
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(buffer_, length),
        [this, self](const boost::system::error_code& ec, std::size_t length) {
            on_write(ec, length);
        });
}

void Session::on_write(const boost::system::error_code& ec, std::size_t length) {
    if (ec) {
        std::cerr << "Write error: " << ec.message() << std::endl;
        return;
    }
    std::cout << "Sent: " << std::string(buffer_, length) << std::endl;
    do_read();
}