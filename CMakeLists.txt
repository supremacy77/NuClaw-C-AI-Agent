cmake_minimum_required(VERSION 3.10)
project(AgentServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost 1.70 REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

add_executable(server
    src/main.cpp
    src/server.cpp
    src/session.cpp
    src/router.cpp
)

target_link_libraries(server
    Boost::headers
    Boost::system
    Threads::Threads
)

target_include_directories(server PRIVATE include)
