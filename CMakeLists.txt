cmake_minimum_required(VERSION 3.24)
project(espresso)

set(CMAKE_CXX_STANDARD 20)

add_executable(espresso main.cpp lib/Server.cpp lib/Server.h lib/HTTPMessage.cpp lib/HTTPMessage.h)
