cmake_minimum_required(VERSION 3.24)
project(espresso)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

add_subdirectory(src)
add_subdirectory(examples)
