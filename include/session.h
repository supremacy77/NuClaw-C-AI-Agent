#include <boost/asio.hpp>
#include <memory>
#include <iostream>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;


class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket)
        : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read();

    void on_read(const boost::system::error_code& ec, std::size_t bytes_transferred);

    void do_write(std::size_t length);

    void on_write(const boost::system::error_code& ec, std::size_t bytes_transferred);

    tcp::socket socket_;
    char buffer_[1024];
};
