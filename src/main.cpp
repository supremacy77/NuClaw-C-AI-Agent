#include "server.h"
#include <iostream>

int main() {
    try {
        asio::io_context io_context;
        Server server(io_context, 8888);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}