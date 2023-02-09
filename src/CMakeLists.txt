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
        PathRegex.cpp
        Route.cpp

        orm/database/SQLiteDatabaseManager.cpp
        orm/sql/SQLGenerator.cpp
        orm/database/DatabaseManager.cpp
        orm/sql/SQLiteSQLGenerator.cpp
        orm/sql/SQLTypes.cpp
        orm/model/fields/BaseModelField.cpp
        orm/model/ModelData.cpp orm/model/ModelData.h orm/model/fields/Field.cpp orm/model/fields/Field.h)

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
        utils.h

        orm/sql/SQLTypes.h
        orm/database/DatabaseManager.h
        orm/database/SQLiteDatabaseManager.h
        orm/exceptions.h
        orm/sql/SQLGenerator.h
        )

# Create a library target for the Espresso library
add_library(espresso STATIC ${SOURCES})

# Add the source files to the library target
target_sources(espresso PRIVATE ${SOURCES})

# Add the include directories to the library target
target_include_directories(espresso PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add the library target to the build tree
add_library(espresso::espresso ALIAS espresso)