// player.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "player.h"
#include "world.h"

#include <string>
#include <utility>

Player::Player(int id, std::string name, int init_pos, int init_fund) :
        id(id), name(std::move(name)), pos(init_pos), spawn_pos(init_pos),
        fund(init_fund) {}

int Player::get_position() const { return this->pos; }

std::string Player::get_name() const { return this->name; }

int Player::get_fund() const { return this->fund; }

void Player::move_to(int target) { this->pos = target; }

bool Player::upd_fund(int add) {
    this->fund += add;
    return this->fund > 0;
}

bool Player::buy_land(CLand *land) {
    if (this->fund >= land->get_cost()) {
        land->set_owner(this->id, this->name);
        this->fund -= land->get_cost();
        return true;
    } else {
        return false;
    }
}

bool Player::upgrade_land(CLand *land) {
    if (this->fund >= land->get_cost()) {
        this->fund -= land->get_cost();
        land->upgrade();
        return true;
    } else {
        return false;
    }
}

int Player::freeze(int round /* = 0 */) {
    this->frozen += round;
    return this->frozen;
}

void Player::add_card(Card *card) {
    this->cards.push_back(card);
}

std::vector<Card *> Player::get_cards() const {
    return this->cards;
}

void Player::remove_card(Card *card) {
    for (auto it = this->cards.begin(); it != this->cards.end(); ++it) {
        if (*it == card) {
            this->cards.erase(it);
            break;
        }
    }
}