// config.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef CONFIG_H_
#define CONFIG_H_

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config {
private:
    struct {
        int init_fund = 0;
    } player_rules;
    struct {
        int default_size = 0;
        double CLand_prob = 0;
        double FLand_prob = 0;
    } world_rules;
    struct {
        double cost_ratio = 0;
        double rent_ratio = 0;
    } building_rules;

    Config() {
        std::ifstream cf("..\\conf\\rules.json");
        json j;
        cf >> j;
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

public:
    Config(const Config &) = delete;

    Config(Config &&) = delete;

    ~Config() = default;

    static Config &config() {
        static Config conf;
        return conf;
    }

    double get_player_rule(const std::string &name) const;
    double get_world_rule(const std::string &name) const;
    double get_building_rule(const std::string &name) const;
};


#endif