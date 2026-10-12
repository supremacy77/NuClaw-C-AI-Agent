#include "router.h"
#include "server.h"
#include <iostream>

int main() {
    try {
        asio::io_context io_context;
        Router router;
        // Add routes to the router

        router.add_route("GET", "/hello", [](const Request& req) {
            return "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        });
        Server server(io_context, 8888, router);
        std::cout << "Server listening on 127.0.0.1:8888" << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}