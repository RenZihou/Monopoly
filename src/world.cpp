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

Building::Building(std::string name, int init_upgrade_cost, int init_rent) :
        name(std::move(name)), upgrade_cost(init_upgrade_cost),
        rent(init_rent) {}

Building::Building(Building &&other) noexcept:
        name(std::move(other.name)), level(other.level),
        upgrade_cost(other.upgrade_cost), rent(other.rent) {}

std::string Building::get_name() const { return this->name; }

int Building::get_cost() const { return this->upgrade_cost; }

int Building::get_rent() const { return this->rent; }

int Building::get_level() const { return this->level; }

bool Building::upgrade() {
    this->upgrade_cost = static_cast<int>(
            this->upgrade_cost * Building::cost_ratio);
    this->rent = static_cast<int>(this->rent * Building::rent);
    this->level += 1;
    return true;
}

Land::Land() : type(VACANT) {}

int Land::get_type() const { return this->type; }

std::string Land::description() { return "vacant land"; }

CLand::CLand(Building *building) : building(building) {
    this->type = COMMERCIAL;
}

std::string CLand::description() {
    std::string des;
    std::string bd_info = this->building->get_name() + " (Lv." +
                          std::to_string(this->building->get_level()) + ")";
    if (this->owner.id == -1) {  // without owner
        des = "vacant " + bd_info;
    } else {
        des = "@" + this->owner.name + "'s " + bd_info;
    }
    return des;
}

int CLand::get_owner() const { return this->owner.id; }

int CLand::get_rent() const { return this->building->get_rent(); }

int CLand::get_cost() const { return this->building->get_cost(); }

void CLand::set_owner(int owner_id, const std::string &owner_name) {
    this->owner = {owner_id, owner_name};
}

void CLand::upgrade() { this->building->upgrade(); }


FLand::FLand(Card *card) : card(card) { this->type = FUNCTIONAL; }

std::string FLand::description() {
//    return "gives '" + this->card->get_name() + "' card";
    return "FLand";  // TODO
}

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
                accu_prob += bd.weight;
                if (b < accu_prob) {  // create bd type CLand
                    auto new_bd = new Building{
                            bd.name, bd.build_cost, bd.rent};
                    new_land = new CLand(new_bd);
                    break;
                }
            }
        } else if (r < (c_prob + f_prob)) { // create functional land
            double b = dist(map_gen);
            double accu_prob = 0;
            for (const auto &c : c_list) {
                accu_prob += c.weight;
                if (b < accu_prob) {  // create c type FLand
                    auto new_card = new Card{c.name, c.description};
                    new_land = new FLand(new_card);
                    break;
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

Land *Map::operator[](int index) {
    return this->map[index];
}

std::vector<Land *>::iterator Map::begin() { return this->map.begin(); }

std::vector<Land *>::iterator Map::end() { return this->map.end(); }

int Map::get_size() const { return this->size; }

