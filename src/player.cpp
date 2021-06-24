// player.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "player.h"

#include <string>

Player::Player(int id, std::string name, int init_pos, int init_fund) :
        id(id), name(std::move(name)), spawn_pos(init_pos), pos(init_pos),
        fund(init_fund) {
    this->skill.skill = SkillFactory::factory().gen_skill();
}

Player::~Player() {
    delete this->skill.skill;
}

int Player::get_position() const { return this->pos; }

std::string Player::get_name() const { return this->name; }

int Player::get_id() const { return this->id; }

bool Player::at_spawn_pos() const { return this->spawn_pos == this->pos; }

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

void Player::cool_down(int r /* = 1 */) {
    this->skill.cd += r;
    this->skill.cd = std::min(this->skill.cd,
                              this->skill.skill->get_cool_down(this->skill.lv));
}

int Player::get_cool_down() const {
    return this->skill.skill->get_cool_down(this->skill.lv) - this->skill.cd;
}

bool Player::can_use_skill(const std::string &condition) const {
    return !this->get_cool_down() && this->skill.skill->has_condition(condition);
}

std::string Player::get_skill_name() const {
    return this->skill.skill->get_name();
}

std::string Player::get_skill_des() const {
    std::string cd_des = this->get_cool_down()
                         ? (" [cool-down after " +
                            std::to_string(this->get_cool_down()) + " rounds]")
                         : " [already cooled-down]";
    return this->skill.skill->get_name() + " (Lv." +
           std::to_string(this->skill.lv) + ")" + cd_des;
}

std::string Player::use_skill() {
    this->skill.cd = 0;
    return this->skill.skill->get_effect(this->skill.lv);
}

bool Player::promote() {
    if (this->skill.lv < this->skill.skill->get_max_lv()) {
        ++this->skill.lv;
        return true;
    } else return false;
}

void Player::transfer(Skill *new_skill) {
    this->skill.skill = new_skill;
    this->skill.lv = 0;
    this->skill.cd = 0;
}