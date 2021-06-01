// game.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "game.h"
//#include "world.h"
//#include "config.h"
//#include "player.h"

#include <iostream>
#include <iomanip>
#include <random>
//#include <utility>
#include <vector>
#include <exception>

void Game::move_player(int player_id, int steps) {
    int prev_pos = this->players[player_id]->get_position();
    this->players_pos.erase(prev_pos);
    this->players[player_id]->move(steps);
    int new_pos = this->players[player_id]->get_position();
    if (this->players_pos.count(new_pos))
        this->move_player(players_pos[new_pos], 1);
    this->players_pos[new_pos] = player_id;
}

void Game::setup(int world_size, int seed,
                 const std::vector<std::string> &player_names) {
    // generate map
    if (world_size < player_names.size())
        throw std::invalid_argument(
                "world size cannot be smaller than player number");
    double CLand_prob = Config::config().get_world_rule("CLand_prob");
    double FLand_prob = Config::config().get_world_rule("FLand_prob");
    this->map = std::move(Map(world_size, seed, CLand_prob, FLand_prob));

    // spawn players
    int init_fund = static_cast<int>(Config::config().get_player_rule(
            "init_fund"));
    int player_id = 0;
    std::mt19937 mt(seed);
    std::uniform_int_distribution<int> pos(0, world_size - 1);
    for (auto &name : player_names) {
        auto *new_player = new Player(player_id, name, 0, init_fund);
        this->players.push_back(new_player);
        this->move_player(player_id, pos(mt));
        ++player_id;
    }
}

void Game::display() {
    std::vector<std::string> descriptions;
    int size = this->map.get_size();
    int d_width = 0;
    for (auto &land : this->map) {
        std::string des = land->description();
        int len = static_cast<int>(des.size());
        d_width = d_width > len ? d_width : len;
        descriptions.push_back(des);
    }

    // print map
    for (int i = 0; i < size; ++i) {
        if (!i) std::cout << "-->";
        else if (i == size - 1) std::cout << "---";
        else std::cout << "|  ";
        std::cout << "[ " << std::left << std::setw(d_width) << descriptions[i]
                  << " ]";
        if (this->players_pos.count(i))  // has player at this position
            std::cout << " @" << this->players[this->players_pos[i]]->get_name();
        std::cout << "\n";
    }
    std::flush(std::cout);
}