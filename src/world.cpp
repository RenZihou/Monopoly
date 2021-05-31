// world.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <string>
#include <utility>
#include <vector>
#include <random>
#include <fstream>

#include <nlohmann/json.hpp>

#include "world.h"

using json = nlohmann::json;

//Building::Building() {
//    this->name = "";
//    this->level = 0;
//    this->upgrade_cost = 0;
//    this->rent = 0;
//}

Building::Building(std::string name_, int init_upgrade_cost, int init_rent) {
    this->name = std::move(name_);
    this->level = 0;
    this->upgrade_cost = init_upgrade_cost;
    this->rent = init_rent;
    this->owner = -1;
}

Building::Building(Building&& other) noexcept {
    this->name = std::move(other.name);
    this->level = other.level;
    this->upgrade_cost = other.upgrade_cost;
    this->rent = other.rent;
    this->owner = other.owner;
}

int Building::get_cost() const {
    return this->upgrade_cost;
}

int Building::get_rent() const {
    return this->rent;
}

bool Building::upgrade() {
    this->upgrade_cost = static_cast<int>(this->upgrade_cost * Building::cost_ratio);
    this->rent = static_cast<int>(this->rent * Building::rent);
    this->level += 1;
}

Land::Land() {
    this->type = VACANT;
}

CLand::CLand(Building& building) {
    this->type = COMMERCIAL;
    this->owner = -1;
//    this->building = std::move(building);
    this->building = &building;
}

void CLand::set_owner(int id) {
    this->owner = id;
}

void CLand::upgrade() {
    this->building->upgrade();
}

int CLand::get_rent() const {
    return building->get_rent();
}

int CLand::get_cost() const {
    return building->get_cost();
}

FLand::FLand() {
    this->type = FUNCTIONAL;
}

Map::Map(int size_, int seed, double c_prob, double f_prob) : size(size_),
                                                              seed(seed) {
    std::ifstream cf("../conf/buildings.json");
    json buildings;
    cf >> buildings;
    std::mt19937 map_gen(seed);
    std::uniform_real_distribution<double> dist(0, 1);
    while (size--) {
        Land *new_land = nullptr;
        double r = dist(map_gen);
        if (r < c_prob) {
            // TODO: create single land
            double b = dist(map_gen);
            double accu_prob = 0;  // accumulate probability
//            new_land = &CLand();
        } else if (r < (c_prob + f_prob)) {
//            new_land = &FLand();
        }
        this->map.push_back(new_land);
    }
}

Map::Map(Map &&other) noexcept : size(other.size), seed(other.seed) {
    this->map = std::move(other.map);
}

Map Map::operator=(Map&& other) {

}