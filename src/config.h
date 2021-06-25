// config.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config {
private:
    json conf;

    Config();

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


#endif // CONFIG_H_