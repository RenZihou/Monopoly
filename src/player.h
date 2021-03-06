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
    const int spawn_pos;
    int pos;
    int fund;
    std::vector<Card *> cards;
    struct {
        Skill *skill;
        int lv = 0;
        int cd = 0;
    } skill;
    int frozen = 0;

public:
    Player(int id, std::string name, int init_pos, int init_fund);

    ~Player();

    int get_position() const;

    std::string get_name() const;

    int get_id() const;

    int get_fund() const;

    bool at_spawn_pos() const;

    void move_to(int target);

    bool upd_fund(int add);

    bool buy_land(CLand *land);

    bool upgrade_land(CLand *land);

    int freeze(int round = 0);

    void add_card(Card *card);

    std::vector<Card *> get_cards() const;

    void remove_card(Card *card);

    void cool_down(int r = 1);

    int get_cool_down() const;

    bool can_use_skill(const std::string &condition) const;

    std::string get_skill_name() const;

    std::string get_skill_des() const;

    std::string use_skill();

    bool promote();

    void transfer(Skill *new_skill);
};

#endif  // PLAYER_H_