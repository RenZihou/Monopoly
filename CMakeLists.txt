cmake_minimum_required(VERSION 3.16)
project(proj)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

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
        src/world.h)
