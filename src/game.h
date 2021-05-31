// game.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef GAME_H_
#define GAME_H_

#include <vector>

#include "config.h"
#include "player.h"
#include "world.h"

class Game {
private:
    std::vector<Player> players;
    Map map;
    Config *config = &Config::config();
    Game() = default;
public:
    Game(const Game&) = delete;
    Game(Game&&) = delete;
    ~Game() = default;
    void setup(int player_num, int world_size, int seed);
//    bool round();
    void display() const;
    static Game &game() {
        static Game game;
        return game;
    }
};

#endif