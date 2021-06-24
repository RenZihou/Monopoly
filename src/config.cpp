// config.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <string>
#include <exception>

#include "config.h"

Config::Config() {
    std::ifstream cf("../conf/rules.json");
    cf >> this->conf;
    cf.close();
};

double Config::get_player_rule(const std::string &name) const {
    return this->conf.at("player").at(name);
}

double Config::get_world_rule(const std::string &name) const {
    return this->conf.at("world").at(name);
}

double Config::get_building_rule(const std::string &name) const {
    return this->conf.at("building").at(name);
}