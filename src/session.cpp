#include "session.h"
#include "router.h"

void Session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(asio::buffer(buffer_),
        [this, self](const boost::system::error_code& ec, std::size_t length) {
            if (ec) {
                return;
            }

            if (parser_.parse(buffer_, length, request_)) {
                handle_request();
            } else {
                do_read();
            }
        });
}

void Session::handle_request() {
    auto match = router_.match(request_.method, request_.path);
    if (match) {
        response_ = match->h(request_);
        // Send the response back to the client
    } else {
        response_ = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
        // Handle 404 Not Found
    }
    do_write(response_.size());
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

    auto it = request_.headers.find("Connection");
    if (it != request_.headers.end() && it->second == "close") {
        std::cout << "Closing connection as per client request." << std::endl;
        socket_.close();
        return;
    } else {
        std::cout << "Waiting for next request..." << std::endl;
        request_ = Request();
        do_read();
    }
    
}