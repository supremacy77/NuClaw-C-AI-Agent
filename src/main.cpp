#include "rule_engine.h"
#include "server.h"
#include <iostream>


int main() {
    try {
        asio::io_context io_context;
        RuleEngine rule_engine;
        
        Server server(io_context, 8888, rule_engine);
        std::cout << "Server listening on 127.0.0.1:8888" << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}