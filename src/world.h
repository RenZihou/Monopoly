// world.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <vector>

#include "card.h"
#include "config.h"

#define VACANT 0
#define COMMERCIAL 1
#define FUNCTIONAL 2

class Building {
private:
    const double cost_ratio = Config::config().get_building_rule("cost_ratio");
    const double rent_ratio = Config::config().get_building_rule("rent_ratio");

    std::string name;
    int level = 0;
    int upgrade_cost = 0;
    int rent = 0;
//    int owner = -1;
public:
    Building(std::string name, int init_upgrade_cost, int init_rent);

    Building(Building &&other) noexcept;

    ~Building() = default;

    std::string get_name() const;

    int get_cost() const;

    int get_rent() const;

    int get_level() const;

    bool upgrade();
};

class Land {
private:
protected:
    int type;
public:
//    int type;

    Land();

    ~Land() = default;

    virtual std::string description();
    int get_type() const;
};

class CLand : public Land {
private:
    struct {
        int id = -1;
        std::string name;
    } owner;
    Building *building;

public:
    explicit CLand(Building *building);

    ~CLand();

    std::string description() override;

    int get_owner() const;
    int get_rent() const;
    int get_cost() const;

    void set_owner(int owner_id, const std::string &owner_name);

    void upgrade();


};

class FLand : public Land {
private:
    Card *card;
public:
    explicit FLand(Card *card);

    ~FLand();

    std::string description() override;

    Card *get_card() const;
};

class Map {
private:
    int size = 0;
    int seed = 0;
    std::vector<Land *> map{};
public:
    Map() = default;

    Map(int size, int seed, double c_prob, double f_prob);

    Map(Map &&other) noexcept;

    Map &operator=(Map &&other) noexcept;

    Land *operator[](int index);

    ~Map() = default;

    std::vector<Land *>::iterator begin();

    std::vector<Land *>::iterator end();

    int get_size() const;
};

#endif