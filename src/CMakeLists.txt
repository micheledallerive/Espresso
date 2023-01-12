set(SOURCES
        Cookies.cpp
        HTTPMessage.cpp
        HTTPMethod.cpp
        HTTPRequest.cpp
        HTTPResponse.cpp
        Middleware.cpp
        Router.cpp
        Server.cpp
        utils.cpp)

set(HEADERS Cookies.h
        espresso.h
        HTTPMessage.h
        HTTPMethod.h
        HTTPRequest.h
        HTTPResponse.h
        Middleware.h
        Router.h
        Server.h
        utils.h)

# Create a library target for the Espresso library
add_library(espresso STATIC ${SOURCES})

# Add the source files to the library target
target_sources(espresso PRIVATE ${SOURCES} ${HEADERS})

# Include the headers directory for the library target
target_include_directories(espresso PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

