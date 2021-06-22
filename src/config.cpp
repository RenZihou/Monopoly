// config.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <string>
#include <exception>

#include "config.h"

Config::Config() {
    std::ifstream cf("../conf/rules.json");
    json j;
    cf >> j;
    cf.close();
    this->player_rules = {
            .init_fund = j.at("player").at("init_fund")
    };
    this->world_rules = {
            .default_size = j.at("world").at("default_size"),
            .CLand_prob = j.at("world").at("CLand_prob"),
            .FLand_prob = j.at("world").at("FLand_prob")
    };
    this->building_rules = {
            .cost_ratio = j.at("building").at("cost_ratio"),
            .rent_ratio = j.at("building").at("rent_ratio")
    };
};

double Config::get_player_rule(const std::string &name) const {
    if (name == "init_fund") return this->player_rules.init_fund;
    throw std::invalid_argument("Config Key Error at Config::get_player_rule");
}

double Config::get_world_rule(const std::string &name) const {
    if (name == "default_size") return this->world_rules.default_size;
    if (name == "CLand_prob") return this->world_rules.CLand_prob;
    if (name == "FLand_prob") return this->world_rules.FLand_prob;
    throw std::invalid_argument("Config Key Error at Config::get_world_rule");
}

double Config::get_building_rule(const std::string &name) const {
    if (name == "cost_ratio") return this->building_rules.cost_ratio;
    if (name == "rent_ratio") return this->building_rules.rent_ratio;
    throw std::invalid_argument(
            "Config Key Error at Config::get_building_rule");
}