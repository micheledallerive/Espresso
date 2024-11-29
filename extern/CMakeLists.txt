# fetch nlohmann json
include(FetchContent)
FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

set(INJA_USE_EMBEDDED_JSON OFF)
FetchContent_Declare(
        inja
        GIT_REPOSITORY https://github.com/pantor/inja.git
        GIT_TAG v3.4.0
)

FetchContent_MakeAvailable(inja)

target_link_libraries(espresso PRIVATE nlohmann_json::nlohmann_json inja)