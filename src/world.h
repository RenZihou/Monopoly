// world.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <vector>

//#include

#define VACANT 0
#define COMMERCIAL 1
#define FUNCTIONAL 2

class Building {
private:
    static const float cost_ratio;
    static const float rent_ratio;

    std::string name;
    int level;
    int upgrade_cost;
    int rent;
public:
    Building();
    ~Building() = default;
    void build(std::string name_, int init_upgrade_cost, int init_rent);
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
    Building building;
public:
    CLand();
    ~CLand() = default;
    void set_owner(int owner_id);
    void upgrade();
    int get_rent() const;
    int get_cost() const;
};

class FLand : public Land {
private:
public:
    FLand();
    ~FLand() = default;

};

class Map {
private:
    int size;
    int seed;
    std::vector<Land> map;
public:
    Map(int size_, int seed, double c_prob, double f_prob);
    ~Map() = default;
//    void spawn_player(int num);
};

#endif