// game.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "game.h"
#include "world.h"
#include "config.h"
#include "player.h"

#include <iostream>
#include <iomanip>
#include <random>
#include <utility>
#include <vector>

void Game::setup(int world_size, int seed,
                 const std::vector<std::string> &player_names) {
    // generate map
    int CLand_prob = static_cast<int>(Config::config().get_world_rule(
            "CLand_prob"));
    int FLand_prob = static_cast<int>(Config::config().get_world_rule(
            "FLand_prob"));
    this->map = std::move(Map(world_size, seed, CLand_prob, FLand_prob));

    // spawn players
    int init_fund = static_cast<int>(Config::config().get_player_rule(
            "init_fund"));
    int player_id = 0;
    std::mt19937 mt(seed);
    std::uniform_int_distribution<int> pos(0, world_size - 1);
    for (auto &name : player_names) {
        auto *new_player = new Player(player_id++, name, pos(mt), init_fund);
        this->players.push_back(new_player);
    }
}

/* TODO
Idea GUI:
-> [ 0][                         ] @Player
|  [ 1][@Alex's housing_unit Lv.3]
|  [ 2][Gives Random Card        ]

...

|  [28][                         ] @Alex
-- [29][                         ]
 */
void Game::display() const {
}