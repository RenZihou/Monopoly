// player.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "player.h"
#include "world.h"

#include <string>
#include <utility>

Player::Player(int id, std::string name, int init_pos, int init_fund) {
    this->id = id;
    this->name = std::move(name);
    this->pos = init_pos;
    this->fund = init_fund;
    this->frozen = 0;
}

void Player::move_to(int new_pos) {
    this->pos = new_pos;
}

bool Player::upd_fund(int add) {
    this->fund += add;
    return this->fund > 0;
}

bool Player::buy_land(CLand &land) {
    if (this->fund >= land.get_cost()) {
        land.set_owner(this->id);
        this->fund -= land.get_cost();
        return true;
    } else {
        return false;
    }
}

bool Player::upd_land(CLand &land) {
    if (this->fund >= land.get_cost()) {
        this->fund -= land.get_cost();
        land.upgrade();
        return true;
    } else {
        return false;
    }
}
