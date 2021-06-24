// skill.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "skill.h"

#include <utility>

Skill::Skill(std::string name, std::string description,
             std::vector<std::string> effect, std::vector<int> cool_down,
             std::vector<std::string> condition)
        : Card(std::move(name), std::move(description),
               std::move(effect), std::move(condition)) {
//    this->name = std::move(name);
//    this->description = std::move(description);
//    this->effect = std::move(effect);
    this->cool_down = std::move(cool_down);
//    this->condition = std::move(condition);
}

std::string Skill::get_effect(int lv) const {
    return Card::get_effect()[lv];
//    return this->effect[lv];
}

int Skill::get_cool_down(int lv) const {
    return this->cool_down[lv];
}

int Skill::get_max_lv() const {
    return static_cast<int>(this->cool_down.size() - 1);
}