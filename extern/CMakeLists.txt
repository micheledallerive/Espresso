# fetch nlohmann json
include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_Declare(
        reflect_cpp
        GIT_REPOSITORY https://github.com/getml/reflect-cpp.git
        GIT_TAG v0.16.0
)

FetchContent_MakeAvailable(nlohmann_json)
FetchContent_MakeAvailable(reflect_cpp)

set(REFLECTCPP_BSOFF OFF) # Optional
set(REFLECTCPP_CBOR OFF) # Optional
set(REFLECTCPP_FLEXBUFFERS OFF) # Optional
set(REFLECTCPP_MSGPACK OFF) # Optional
set(REFLECTCPP_TOML OFF) # Optional
set(REFLECTCPP_UBJSOFF OFF) # Optional
set(REFLECTCPP_XML OFF) # Optional
set(REFLECTCPP_YAML OFF) # Optional
# import sqlite3
find_package(SQLite3 REQUIRED)

function(espresso_link_external TARGET_NAME)
    target_link_libraries( ${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${TARGET_NAME} PRIVATE reflectcpp)
    target_link_libraries(${TARGET_NAME} PRIVATE sqlite3)
endfunction()