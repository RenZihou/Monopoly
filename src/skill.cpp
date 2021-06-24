// skill.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "skill.h"

#include <fstream>
#include <random>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Skill::Skill(std::string name, std::string description,
             std::vector<std::string> effect, std::vector<int> cool_down,
             std::vector<std::string> condition)
        : Card(std::move(name), std::move(description),
               std::move(effect), std::move(condition)) {
    this->cool_down = std::move(cool_down);
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

using json = nlohmann::json;

void from_json(const json &j, S &s) {
    j.at("name").get_to(s.name);
    j.at("description").get_to(s.description);
    j.at("effect").get_to(s.effect);
    j.at("cool_down").get_to(s.cool_down);
    j.at("condition").get_to(s.condition);
}

SkillFactory::SkillFactory() {
    json skills;
    std::ifstream file("../conf/skills.json");
    file >> skills;
    skills.get_to(this->skill_list);
    file.close();

    this->count = static_cast<int>(this->skill_list.size());
}

Skill *SkillFactory::gen_skill() const {
    std::random_device rd{};
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> s_dist(0, this->count - 1);
    const S *s = &this->skill_list[s_dist(mt)];
    auto skill = new Skill(s->name, s->description, s->effect,
                           s->cool_down, s->condition);
    return skill;
}