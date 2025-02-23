# fetch nlohmann json
include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

find_package(SQLite3 REQUIRED)

if (USE_OPENSSL)
    find_package(OpenSSL REQUIRED)
else()
    message(WARNING "OpenSSL is not enabled: the Espresso HTTPS API will not be available.")
endif ()

function(espresso_link_external TARGET_NAME)
    target_link_libraries(${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${TARGET_NAME} PRIVATE sqlite3)
    if (USE_OPENSSL)
        target_link_libraries(${TARGET_NAME} PRIVATE ssl crypto)
    endif ()
endfunction()