// card.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <string>
#include "card.h"

Card::Card(std::string name, std::string description) {
    this->name = std::move(name);
    this->description = std::move(description);
}

std::string Card::get_name() const { return this->name; }
