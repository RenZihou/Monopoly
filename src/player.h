// player.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <vector>

#include "card.h"
#include "skill.h"

class Player {
private:
    std::string name;
    int pos;
    int fund;
    std::vector<Card> cards;
    // std::vector<SKill> skill;
    int frozen;

public:
    Player();
    ~Player();
    void move(int steps);
    void upd_fund(int add);
    void buy_land(int pos);
    void upd_land(int pos);
};

#endif