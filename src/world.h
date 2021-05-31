// world.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <vector>

#include "card.h"

#define VACANT 0
#define COMMERCIAL 1
#define FUNCTIONAL 2

class Building {
private:
    const double cost_ratio = 1.4;
    const double rent_ratio = 1.5;

    std::string name;
    int level = 0;
    int upgrade_cost = 0;
    int rent = 0;
    int owner = -1;
public:
    Building(std::string name_, int init_upgrade_cost, int init_rent);

    Building(Building &&other) noexcept;

    ~Building() = default;

    int get_cost() const;

    int get_rent() const;

    bool upgrade();
};

class Land {
private:
public:
    int type;

    Land();

    ~Land() = default;
};

class CLand : public Land {
private:
    int owner;
    Building *building;
public:
    explicit CLand(Building &building);

    ~CLand() = default;

    void set_owner(int owner_id);

    void upgrade();

    int get_rent() const;

    int get_cost() const;
};

class FLand : public Land {
private:
    Card *card;
public:
    explicit FLand(Card &card);

    ~FLand() = default;

};

class Map {
private:
    int size = 0;
    int seed = 0;
    std::vector<Land *> map{};
public:
    Map() = default;

    Map(int size_, int seed, double c_prob, double f_prob);

    Map(Map &&other) noexcept;

    Map &operator=(Map &&other) noexcept;

    ~Map() = default;
//    void spawn_player(int num);
};

#endif