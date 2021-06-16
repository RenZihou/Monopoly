// game.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "game.h"
//#include "world.h"
//#include "config.h"
//#include "player.h"

#include <iostream>
#include <iomanip>
#include <random>
//#include <utility>
#include <vector>
#include <exception>
#include <termio.h>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>


bool Game::_moveto(int player_id, int target) {
    if (target >= this->map.get_size()) return false;
    Player *player = this->players[player_id];
    int prev_pos = player->get_position();
    this->players_pos.erase(prev_pos);
    player->move_to(target);
    if (this->players_pos.count(target))
        this->_move(players_pos[target], 1);
    this->players_pos[target] = player_id;
    return true;
}

bool Game::_move(int player_id, int steps) {
    Player *player = this->players[player_id];
    int target = player->get_position() + steps;
    target %= this->map.get_size();
    this->_moveto(player_id, target);
    return true;
}

bool Game::_buy(int player_id, int) {

}

bool Game::exec(std::vector<std::string> cmd) {
    auto it = cmd.begin();
    std::string name = *it;
    if (name == "move") {
        int player_id = std::stoi(*(++it));
        int steps = std::stoi(*(++it));
        return this->_move(player_id, steps);
    } else if (name == "moveto") {
        int player_id = std::stoi(*(++it));
        int target = std::stoi(*(++it));
        return this->_moveto(player_id, target);
    }
}

void Game::setup(int world_size, int seed,
                 const std::vector<std::string> &player_names) {
    // generate map
    if (world_size < player_names.size())
        throw std::invalid_argument(
                "world size cannot be smaller than player number");
    double CLand_prob = Config::config().get_world_rule("CLand_prob");
    double FLand_prob = Config::config().get_world_rule("FLand_prob");
    this->map = std::move(Map(world_size, seed, CLand_prob, FLand_prob));

    // spawn players
    int init_fund = static_cast<int>(Config::config().get_player_rule(
            "init_fund"));
    int player_id = 0;
    std::mt19937 mt(seed);
    std::uniform_int_distribution<int> pos(0, world_size - 1);
    for (auto &name : player_names) {
        auto *new_player = new Player(player_id, name, 0, init_fund);
        this->players.push_back(new_player);
        this->_move(player_id, pos(mt));
        ++player_id;
    }
}

Game *Game::cycle(int player_id) {
    Player *player = this->players[player_id];
    // print the map and player info
    this->display(player_id);

    // player instruction
    std::cout << "Make Your Decision! "
              << "(type <r> for roll, <c> for card, <h> for help)\n";
    while (true) {
        int key = Game::keyboard();
        if (key == 'r') {  // roll to move
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> roll(1, 6);
            int steps = roll(mt);
//            printf("You got a roll for point %d", steps);
            this->_move(player_id, roll(mt));
            break;
        } else if (key == 'c') {  // TODO: use card
            break;
        } else if (key == 'h') {  // TODO: print manual
            break;
        }
#ifdef CHEAT_ON  // only available when CHEAT_ON flag is on
        else if (key == '0') {  // cheat mode
            std::string command;
            std::vector<std::string> cmd;
            std::cout << "> ";
            getline(std::cin, command);
            std::stringstream commandss(command);
            while (commandss >> command) cmd.push_back(command);
            this->exec(cmd);
            break;
        }
#endif
    }
    this->display(player_id);

    // check new land
    Land *new_pos_ = this->map[player->get_position()];
    if (new_pos_->get_type() == COMMERCIAL) {
        auto *new_pos = dynamic_cast<CLand *>(new_pos_);
        if (new_pos->get_owner() == -1) {
            // vacant land, buy or not
            std::cout << "You reached a vacant commercial land!"
                      << "(type <b> to buy it, <p> to do nothing)\n";
            switch (Game::keyboard()) {
                case 'b':
                    if (player->buy_land(new_pos)) {
                        std::cout << "You bought it successfully!\n";
                    } else {
                        std::cout << "You have no enough money :(\n";
                    }
                    break;
                case 'p':
                    break;
                default:
                    // invalid input
                    ;
            }
        } else if (new_pos->get_owner() == player_id) {
            // player's land, upgrade or not
            std::cout << "You reached your own land!"
                      << "(type <u> to upgrade your building, <p> to do nothing\n";
            switch (Game::keyboard()) {
                case 'u':
                    if (player->upd_land(new_pos)) {
                        std::cout << "You upgraded it successfully!";
                    } else {
                        std::cout << "You have no enough money :(";
                    }
            }
        } else {
            // other's land, pay rent
            int rent = new_pos->get_rent();
            player->upd_fund(-rent);
            this->players[new_pos->get_owner()]->upd_fund(rent);
            // TODO: check broke
        }
    } else if (new_pos_->get_type() == FUNCTIONAL) {}
    return this;
}

Game *Game::round() {

    return this;
}

void Game::display() {
    std::vector<std::string> descriptions;
    int size = this->map.get_size();
    int d_width = 0;
    for (auto &land : this->map) {
        std::string des = land->description();
        int len = static_cast<int>(des.size());
        d_width = d_width > len ? d_width : len;
        descriptions.push_back(des);
    }

    // print map
    for (int i = 0; i < size; ++i) {
        if (!i) std::cout << "-->";
        else if (i == size - 1) std::cout << "---";
        else std::cout << "|  ";
        std::cout << "[ " << std::left << std::setw(d_width) << descriptions[i]
                  << " ]";
        if (this->players_pos.count(i))  // has player at this position
            std::cout << " @"
                      << this->players[this->players_pos[i]]->get_name();
        std::cout << "\n";
    }
    std::flush(std::cout);
}

void Game::display(int player_id) {
    system("clear");  // clear the console
    this->display();
    std::cout << "\n==== CURRENT PLAYER: "
              << this->players[player_id]->get_name() << " ====\n";
    std::cout << "Your Fund: " << this->players[player_id]->get_fund() << "\n";
    // TODO: print cards, skills
    std::flush(std::cout);
}

int Game::keyboard() {
    // https://blog.csdn.net/u013467442/article/details/51173441
    int in;
    struct termios new_settings{};
    struct termios stored_settings{};
    tcgetattr(0, &stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0, &stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);

    while (true) {
        in = getchar();
        tcsetattr(0, TCSANOW, &stored_settings);
        return in;
    }
}