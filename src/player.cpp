// player.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "player.h"
#include "world.h"

#include <string>
#include <utility>
#include <fstream>
#include <random>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct S {
    std::string name;
    std::string description;
    std::vector<std::string> effect;
    std::vector<int> cool_down;
    std::vector<std::string> condition;
};

void from_json(const json &j, S &s) {
    j.at("name").get_to(s.name);
    j.at("description").get_to(s.description);
    j.at("effect").get_to(s.effect);
    j.at("cool_down").get_to(s.cool_down);
    j.at("condition").get_to(s.condition);
}

Player::Player(int id, std::string name, int init_pos, int init_fund) :
        id(id), name(std::move(name)), pos(init_pos), spawn_pos(init_pos),
        fund(init_fund) {
    json skills;
    std::vector<S> s_list;
    std::ifstream sf("../conf/skills.json");
    sf >> skills;
    skills.get_to(s_list);
    sf.close();

    int tot_skill = static_cast<int>(s_list.size());
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> s_dist(0, tot_skill - 1);
    S *s = &s_list[s_dist(mt)];
    this->skill.skill = new Skill{s->name, s->description, s->effect,
                                  s->cool_down, s->condition};
}

Player::~Player() {
    delete this->skill.skill;
}

int Player::get_position() const { return this->pos; }

std::string Player::get_name() const { return this->name; }

int Player::get_id() const { return this->id; }

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

bool Player::promote() {
    if (this->skill.lv < this->skill.skill->get_max_lv()) {
        ++this->skill.lv;
        return true;
    } else return false;
}