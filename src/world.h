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
//    int owner = -1;
public:
    Building(std::string name, int init_upgrade_cost, int init_rent);

    Building(Building &&other) noexcept;

    ~Building() = default;

    std::string get_name() const;

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

    virtual std::string description();
};

class CLand : public Land {
private:
    struct {
        int id = -1;
        std::string name;
    } owner;
    Building *building;

public:
    explicit CLand(Building &building);

    ~CLand() = default;

    std::string description() override;

    void set_owner(int owner_id, const std::string &owner_name);

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

    std::string description() override;
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

    ~Map() = default;

    std::vector<Land *>::iterator begin() const;

    std::vector<Land *>::iterator end() const;

    int get_size() const;
};

#endif