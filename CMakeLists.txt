cmake_minimum_required(VERSION 3.16)
project(Monopoly)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3
        GIT_SHALLOW)
FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(Monopoly src/main.cpp
        src/card.cpp src/card.h
        src/game.cpp src/game.h
        src/player.cpp src/player.h
        src/skill.cpp src/skill.h
        src/world.cpp src/world.h
        src/config.cpp src/config.h)

target_link_libraries(Monopoly PRIVATE nlohmann_json::nlohmann_json)

add_definitions(-DCHEAT_ON)

