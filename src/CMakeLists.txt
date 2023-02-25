set(SOURCES
        Cookies.cpp
        HTTPMessage.cpp
        HTTPMethod.cpp
        HTTPRequest.cpp
        HTTPResponse.cpp
        Middleware.cpp
        Router.cpp
        Server.cpp
        utils.cpp
        PathRegex.cpp Route.cpp

        lib/json/JSONBase.cpp
        lib/json/JSONLiteral.cpp
        lib/json/JSONBoolean.cpp
        lib/json/JSONNumber.cpp
        lib/json/JSONArray.cpp
        )

set(HEADERS Cookies.h
        HTTPMessage.h
        HTTPMethod.h
        HTTPRequest.h
        HTTPResponse.h
        Middleware.h
        Router.h
        Server.h
        PathRegex.h
        Route.h
        utils.h)

# Create a library target for the Espresso library
add_library(espresso STATIC ${SOURCES})

# Add the source files to the library target
target_sources(espresso PRIVATE ${SOURCES})

# Add the include directories to the library target
target_include_directories(espresso PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add the library target to the build tree
add_library(espresso::espresso ALIAS espresso)