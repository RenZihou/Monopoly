// game.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "game.h"
#include "world.h"
#include "config.h"

#include <utility>

void Game::setup(int player_num, int world_size, int seed) {
    int CLand_prob = static_cast<int>(Config::config().get_world_rule(
            "CLand_prob"));
    int FLand_prob = static_cast<int>(Config::config().get_world_rule(
            "FLand_prob"));
    this->map = std::move(Map(world_size, seed, CLand_prob, FLand_prob));
}

void Game::display() const {
    static int rows = 0;  // TODO: not finished
}