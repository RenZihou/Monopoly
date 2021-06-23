// card.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <string>
#include <algorithm>
#include "card.h"

Card::Card(std::string name, std::string description,
           std::vector<std::string> effect, std::vector<std::string> condition) {
    this->name = std::move(name);
    this->description = std::move(description);
    this->effect = std::move(effect);
    this->condition = std::move(condition);
}

std::string Card::get_name() const { return this->name; }

bool Card::has_condition(const std::string &cond) const {
    return std::find(this->condition.begin(), this->condition.end(), cond)
           != this->condition.end();
}

std::vector<std::string> Card::get_effect() const {
    return this->effect;
}