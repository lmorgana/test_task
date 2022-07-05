cmake_minimum_required(VERSION 3.21)
project(tz)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tz
        main.cpp
        server.cpp
        server.hpp
        sockets.cpp
        sockets.hpp
        session.cpp
        server.hpp)
