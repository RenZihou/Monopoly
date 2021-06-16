cmake_minimum_required(VERSION 3.16)
project(proj)

set(CMAKE_CXX_STANDARD 14)

find_package(nlohmann_json 3.9.1 REQUIRED)

include_directories(src)

add_definitions(-DCHEAT_ON)

add_executable(proj
        src/card.cpp
        src/card.h
        src/game.cpp
        src/game.h
        src/main.cpp
        src/player.cpp
        src/player.h
        src/skill.cpp
        src/skill.h
        src/world.cpp
        src/world.h src/config.h src/config.cpp)
