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
    int spawn_pos;
    int pos;
    int fund;
    std::vector<Card *> cards;
    Skill skill;
    int frozen = 0;

public:
    Player(int id, std::string name, int init_pos, int init_fund);

    ~Player() = default;

    int get_position() const;

    std::string get_name() const;

    int get_fund() const;

    void move_to(int target);

    bool upd_fund(int add);

    bool buy_land(CLand *land);

    bool upgrade_land(CLand *land);

    int freeze(int round = 0);

    void add_card(Card *card);

    void use_card();

    void use_skill();

    void promote();
};

#endif