// world.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <string>
#include <utility>
#include <vector>
#include <random>

#include "world.h"

Building::Building() {
    this->name = "";
    this->level = 0;
    this->upgrade_cost = 0;
    this->rent = 0;
}

void Building::build(std::string name_, int init_upgrade_cost, int init_rent) {
    this->name = std::move(name_);
    this->level = 0;
    this->upgrade_cost = init_upgrade_cost;
    this->rent = init_rent;
}

int Building::get_cost() const {
    return this->upgrade_cost;
}

int Building::get_rent() const {
    return this->rent;
}

bool Building::upgrade() {
    this->upgrade_cost *= Building::cost_ratio;
    this->rent *= Building::rent_ratio;
    this->level += 1;
}

Land::Land() {
    this->type = VACANT;
}

CLand::CLand() {
    this->type = COMMERCIAL;
    this->owner = -1;
//    this->building;
}

void CLand::set_owner(int id) {
    this->owner = id;
}

void CLand::upgrade() {
    this->building.upgrade();
}

int CLand::get_rent() const {
    return building.get_rent();
}

int CLand::get_cost() const {
    return building.get_cost();
}

FLand::FLand() {
    this->type = FUNCTIONAL;
}

Map::Map(int size_, int seed, double c_prob, double f_prob) {
    this->size = size_;
    this->seed = seed;
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> dist(0, 1);
    while (size--) {
        Land new_land;
        double r = dist(mt);
        if (r < c_prob) {
            // TODO: create single land
//            new_land = &CLand();
        } else if (r < (c_prob + f_prob)) {
//            new_land = &FLand();
        }
        this->map.push_back(new_land);
    }
}

//void Map::spawn_player(int num) {
//    std::mt19937 mt(this->seed);
//    std::uniform_int_distribution<int> dist(0, this->size - 1);
//
//}