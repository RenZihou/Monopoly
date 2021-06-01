// main.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <cstdio>
#include <string>
#include <vector>
#include "game.h"

int main() {
    // DEBUG: begin
    int seed = 1;
    int world_size = 20;
    std::vector<std::string> player_names {"Player", "Alex", "Amiee"};
    Game::game().setup(world_size, seed, player_names);
    Game::game().display();
    // DEBUG: end
    return 0;
}