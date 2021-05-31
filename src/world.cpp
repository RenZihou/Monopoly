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

struct Bd {
    std::string name;
    int build_cost;
    int rent;
    double weight;
};

void from_json(const json &j, Bd &bd) {
    j.at("name").get_to(bd.name);
    j.at("build_cost").get_to(bd.build_cost);
    j.at("rent").get_to(bd.rent);
    j.at("weight").get_to(bd.weight);
}

struct C {
    std::string name;
    std::string description;
    double weight;
};

void from_json(const json &j, C &c) {
    j.at("name").get_to(c.name);
    j.at("description").get_to(c.description);
    j.at("weight").get_to(c.weight);
}

//Building::Building() {
//    this->name = "";
//    this->level = 0;
//    this->upgrade_cost = 0;
//    this->rent = 0;
//}

Building::Building(std::string name, int init_upgrade_cost, int init_rent) :
        name(std::move(name)), upgrade_cost(init_upgrade_cost),
        rent(init_rent) {}

Building::Building(Building &&other) noexcept:
        name(std::move(other.name)), level(other.level),
        upgrade_cost(other.upgrade_cost),
        rent(other.rent), owner(other.owner) {}

std::string Building::get_name() const { return this->name; }

int Building::get_cost() const { return this->upgrade_cost; }

int Building::get_rent() const { return this->rent; }

bool Building::upgrade() {
    this->upgrade_cost = static_cast<int>(this->upgrade_cost *
                                          Building::cost_ratio);
    this->rent = static_cast<int>(this->rent * Building::rent);
    this->level += 1;
    return true;
}

Land::Land() : type(VACANT) {}

std::string Land::description() {
    return "";
}

CLand::CLand(Building &building) : building(&building) {
    this->type = COMMERCIAL;
}

std::string CLand::description() {
    if (this->owner == -1) {  // without owner
        return "vacant commercial land";
    }
//    return strcat("@", "[name]", "'s ", this->building.name);  // TODO
}

void CLand::set_owner(int id) { this->owner = id; }

void CLand::upgrade() { this->building->upgrade(); }

int CLand::get_rent() const { return building->get_rent(); }

int CLand::get_cost() const { return building->get_cost(); }

FLand::FLand(Card &card) : card(&card) { this->type = FUNCTIONAL; }

Map::Map(int size, int seed, double c_prob, double f_prob) : size(size),
                                                             seed(seed) {
    std::ifstream bf("../conf/buildings.json"),
            cf("../conf/cards.json");
    json buildings,
            cards;
    std::vector<Bd> bd_list;
    std::vector<C> c_list;
    bf >> buildings;
    cf >> cards;
    buildings.get_to(bd_list);
    cards.get_to(c_list);
    bf.close();
    cf.close();

    std::mt19937 map_gen(seed);
    std::uniform_real_distribution<double> dist(0, 1);
    while (size--) {
        Land *new_land = nullptr;
        double r = dist(map_gen);
        if (r < c_prob) {  // create commercial land
            double b = dist(map_gen);
            double accu_prob = 0;  // accumulate probability
            for (const auto &bd : bd_list) {
                if (b < (accu_prob += bd.weight)) {  // create bd type CLand
                    Building new_bd{bd.name, bd.build_cost, bd.rent};
                    new_land = new CLand(new_bd);
                }
            }
        } else if (r < (c_prob + f_prob)) { // create functional land
            double b = dist(map_gen);
            double accu_prob = 0;
            for (const auto &c : c_list) {
                if (b < (accu_prob += c.weight)) {  // create c type FLand
                    Card new_card(c.name, c.description);
                    new_land = new FLand(new_card);
                }
            }
        } else {  // create vacant land
            new_land = new Land();
        }
        this->map.push_back(new_land);
    }
}

Map::Map(Map &&other) noexcept: size(other.size), seed(other.seed),
                                map(std::move(other.map)) {}

Map &Map::operator=(Map &&other) noexcept {
    this->size = other.size;
    this->seed = other.seed;
    this->map = std::move(other.map);
    return *this;
}