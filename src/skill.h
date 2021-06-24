// skill.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef SKILL_H_
#define SKILL_H_

#include <string>
#include <vector>

#include "card.h"

class Skill : public Card {
private:
//    std::string name;
//    std::string description;
//    std::vector<std::string> effect;
    std::vector<int> cool_down;
//    std::vector<std::string> condition;

public:
    Skill(std::string name, std::string description,
          std::vector<std::string> effect, std::vector<int> cool_down,
          std::vector<std::string> condition);

    ~Skill() = default;

//    std::string get_name() const;
    using Card::get_name;
    using Card::get_des;
    using Card::has_condition;

    std::string get_effect(int lv) const;

    int get_cool_down(int lv) const;

    int get_max_lv() const;

//    std::string get_des() const;
};

#endif