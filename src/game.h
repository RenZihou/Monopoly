// game.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef GAME_H_
#define GAME_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "config.h"
#include "player.h"
#include "world.h"

class Game {
private:
    std::vector<Player *> players;
    std::unordered_map<int, int> players_pos;  // {pos, player_id}
    Map map;
    Config *config = &Config::config();

    Game() = default;
    void move_player(int player_id, int steps);

public:
    Game(const Game &) = delete;

    Game(Game &&) = delete;

    ~Game() = default;

    void setup(int world_size, int seed,
               const std::vector<std::string> &player_names);

//    bool round();
    void display();

    static Game &game() {
        static Game game;
        return game;
    }
};

#endif