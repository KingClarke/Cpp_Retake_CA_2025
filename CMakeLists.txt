cmake_minimum_required(VERSION 3.30)
project(C__Retake)

set(CMAKE_CXX_STANDARD 23)

add_executable(C__Retake main.cpp
        BugApp.cpp
        Board.cpp
        Crawler.cpp
        Hopper.cpp
        Slitherer.cpp
        CustomerApp.cpp
        Customer.cpp
)
