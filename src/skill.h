// skill.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef SKILL_H_
#define SKILL_H_

#include <string>
#include <vector>

class Skill {
private:
    std::string name;
    std::string description;
    std::vector<std::string> effect;
    std::vector<int> cool_down;
    std::vector<std::string> condition;

public:
    Skill(std::string name, std::string description,
          std::vector<std::string> effect, std::vector<int> cool_down,
          std::vector<std::string> condition);

    ~Skill() = default;

    std::string get_name() const;

    std::string get_des() const;
};

#endif