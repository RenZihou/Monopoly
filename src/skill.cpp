// skill.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "skill.h"

Skill::Skill(std::string name, std::string description,
             std::vector<std::string> effect, std::vector<int> cool_down,
             std::vector<std::string> condition) {
    this->name = std::move(name);
    this->description = std::move(description);
    this->effect = std::move(effect);
    this->cool_down = std::move(cool_down);
    this->condition = std::move(condition);
}

std::string Skill::get_name() const {
    return this->name;
}

std::string Skill::get_des() const {
    return this->description;
}