cmake_minimum_required(VERSION 3.23)
project(ircserv CXX)

set(CMAKE_CXX_STANDARD )

add_executable(
        main.cpp
        Client.hpp
        Channel.hpp
        Server.hpp
        Client.cpp
        Channel.cpp
        Server.cpp

        )