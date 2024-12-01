cmake_minimum_required(VERSION 3.25)

# use clang
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
# set stdlib to libc++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")

# if it is a debug release, compile with -O0 and -g
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

project(espresso VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(examples)
