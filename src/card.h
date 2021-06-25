// card.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef CARD_H_
#define CARD_H_

#include <string>
#include <vector>

class Card {
private:
    std::string name;
    std::string description;
    std::vector<std::string> effect;
    std::vector<std::string> condition;

public:
    Card(std::string name, std::string description,
         std::vector<std::string> effect, std::vector<std::string> condition);

    ~Card() = default;

    std::string get_name() const;

    std::string get_des() const;

    bool has_condition(const std::string &cond) const;

    std::vector<std::string> get_effect() const;
};

#endif  // CARD_H_