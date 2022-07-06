cmake_minimum_required(VERSION 3.21)
project(tz)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(tz
        header.h
        main.cpp
        server.cpp
        server.hpp
        socket.cpp
        socket.hpp
        session.cpp
        server.hpp
        FdListener.hpp
        FdListener.cpp
        EventSelector.cpp
        EventSelector.hpp
        logg.cpp
        logg.hpp
        )