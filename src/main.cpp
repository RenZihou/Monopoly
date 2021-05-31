// main.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <cstdio>
#include "game.h"

int main() {
    // DEBUG: begin
    int seed = 1;
    int world_size = 20;
    int player = 1;
    Game::game().setup(player, world_size, seed);
    // DEBUG: end
    return 0;
}