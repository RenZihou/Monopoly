// card.h
// -*- encoding: utf-8 -*-
// @Author: RZH

#ifndef CARD_H_
#define CARD_H_

#include <string>

class Card {
    private:
    std::string name;
    std::string description;
public:
    Card(std::string name, std::string description);
    ~Card() = default;

    std::string get_name() const;
};


#endif