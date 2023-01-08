cmake_minimum_required(VERSION 3.24)
project(espresso)

set(CMAKE_CXX_STANDARD 20)

set(TARGET_FILES Server.cpp HTTPMessage.cpp Server.h HTTPMessage.h HTTPRequest.cpp HTTPRequest.h Router.cpp Router.h HTTPResponse.cpp HTTPResponse.h Middleware.cpp Middleware.h HTTPMethod.cpp HTTPMethod.h utils.cpp utils.h)

add_executable(
        espresso
        main.cpp
        ${TARGET_FILES}
)

add_library(
        espresso_lib
        ${TARGET_FILES}
)