cmake_minimum_required(VERSION 3.24)
project(espresso)

# if it is a debug release, compile with -O0 and -g
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

add_subdirectory(src)
add_subdirectory(examples)
