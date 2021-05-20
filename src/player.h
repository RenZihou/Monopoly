// player.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <vector>

#include "card.h"
#include "skill.h"
#include "world.h"

class Player {
private:
    int id;
    std::string name;
    int pos;
    int fund;
    std::vector<Card> cards;
    std::vector<Skill> skill;
    int frozen;

public:
    Player(int id, std::string name, int init_pos, int init_fund);
    ~Player() = default;
    void move_to(int new_pos);
    bool upd_fund(int add);
    bool buy_land(CLand &land);
    bool upd_land(CLand &land);
    void use_card();
    void use_skill();
    void promote();
};

#endif