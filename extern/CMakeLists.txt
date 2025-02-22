# fetch nlohmann json
include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

# import sqlite3
find_package(SQLite3 REQUIRED)

# import openssl using FindOpenSSL
find_package(OpenSSL REQUIRED)

function(espresso_link_external TARGET_NAME)
    target_link_libraries(${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${TARGET_NAME} PRIVATE sqlite3)
    target_link_libraries(${TARGET_NAME} PRIVATE ssl crypto)
endfunction()