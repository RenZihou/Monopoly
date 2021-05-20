// game.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef GAME_H_
#define GAME_H_

#include <vector>

#include "player.h"
#include "world.h"

class Game {
private:
    std::vector<Player> players;
    Map map;
    Game();
public:
    Game(const game&) = delete;
    Game(Game&&) = delete;
    ~Game() = default;
    void setup();
    bool round();
};

#endif