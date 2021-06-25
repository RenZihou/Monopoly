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
    std::vector<int> cool_down;

public:
    Skill(std::string name, std::string description,
          std::vector<std::string> effect, std::vector<int> cool_down,
          std::vector<std::string> condition);

    ~Skill() = default;

    using Card::get_name;
    using Card::get_des;
    using Card::has_condition;

    std::string get_effect(int lv) const;

    int get_cool_down(int lv) const;

    int get_max_lv() const;
};

struct S {
    std::string name;
    std::string description;
    std::vector<std::string> effect;
    std::vector<int> cool_down;
    std::vector<std::string> condition;
};

class SkillFactory {
private:
    std::vector<S> skill_list;
    int count;

    SkillFactory();

public:
    Skill *gen_skill() const;

    static SkillFactory &factory() {
        static SkillFactory sf;
        return sf;
    }
};

#endif  // SKILL_H_
