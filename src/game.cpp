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

bool Game::_setowner(int pos, int player_id) {
    CLand *land = dynamic_cast<CLand *>(this->map[pos]);
    land->set_owner(player_id, this->players[player_id]->get_name());
    return true;
}

bool Game::_setfund(int player_id, int new_fund) {
    Player *player = this->players[player_id];
    int add = new_fund - player->get_fund();
    player->upd_fund(add);
    return true;
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
    } else if (name == "setfund") {
        int player_id = std::stoi(*(++it));
        int fund = std::stoi(*(++it));
        return this->_setfund(player_id, fund);
    } else if (name == "setowner") {
        int pos = std::stoi(*(++it));
        int player_id = std::stoi(*(++it));
        return this->_setowner(pos, player_id);
    }
    return true;
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
    this->alive = static_cast<int>(player_names.size());
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
            this->_move(player_id, steps);
            break;
        } else if (key == 'c') {  // TODO: use card
            break;
        } else if (key == 'h') {  // TODO: print manual
            break;
        }
#ifdef CHEAT_ON  // only available when CHEAT_ON flag is on
        else if (key == '0') {  // cheat mode
            while (true) {
                std::string command;
                std::vector<std::string> cmd;
                std::cout << "> ";
                getline(std::cin, command);
                std::stringstream commandss(command);
                while (commandss >> command) cmd.push_back(command);
                if (cmd[0] == "quit") break;
                else this->exec(cmd);
            }
            break;
        }
#endif
    }
    this->display(player_id);

    // check new land
    Land *curr_land_ = this->map[player->get_position()];
    if (curr_land_->get_type() == COMMERCIAL) {
        auto *curr_land = dynamic_cast<CLand *>(curr_land_);
        if (curr_land->get_owner() == -1) {
            // vacant land, buy or not
            std::cout << "You reached a vacant commercial land!"
                      << "(type <b> to buy it, <p> to do nothing)\n";
            while (true) {
                int key = Game::keyboard();
                if (key == 'b') {
                    if (player->buy_land(curr_land)) {
                        std::cout << "You bought it successfully!\n";
                    } else {
                        std::cout << "You have no enough money :(\n";
                    }
                    break;
                } else if (key == 'p') break;
            }
        } else if (curr_land->get_owner() == player_id) {
            // player's land, upgrade or not
            std::cout << "You reached your own land!"
                      << "(type <u> to upgrade your building, <p> to do nothing\n";
            while (true) {
                int key = Game::keyboard();
                if (key == 'u') {
                    if (player->upgrade_land(curr_land)) {
                        std::cout << "You upgraded it successfully!\n";
                    } else {
                        std::cout << "You have no enough money :(\n";
                    }
                    break;
                } else if (key == 'p') break;
            }
        } else {
            // other's land, pay rent
            int rent = curr_land->get_rent();
            this->players[curr_land->get_owner()]->upd_fund(rent);
            player->upd_fund(-rent);
        }
    } else if (curr_land_->get_type() == FUNCTIONAL) {}
    return this;
}

Game *Game::run() {
    int player_id = 0;
    int tot_player = static_cast<int>(this->players.size());
    while (this->alive > 1) {
        if (this->players[player_id] != nullptr) this->cycle(player_id);

//        for (auto &player : players) {
        for (int pid = 0; pid < tot_player; ++pid) {
            Player *player = this->players[pid];
            if (player != nullptr && player->get_fund() < 0) {
                std::cout << player->get_name() << " goes broke!\n";
                // remove death player from position list
                for (auto it = this->players_pos.begin();
                     it != this->players_pos.end(); ++it) {
                    if (it->second == pid) {
                        this->players_pos.erase(it);
                        break;
                    }
                }
                // deal with death player's houses
                for (auto &land_ : this->map) {
                    if (land_->get_type() == COMMERCIAL) {
                        auto land = dynamic_cast<CLand *>(land_);
                        if (land->get_owner() == pid) {
                            land->set_owner(-1, "");
                        }
                    }
                }
                this->players[pid] = nullptr;
                --this->alive;
            }
        }
        player_id += 1;
        player_id %= tot_player;
    }

    std::string winner_name;
    for (auto player : this->players) {
        if (player != nullptr) winner_name = player->get_name();
    }
    system("clear");
    this->display();
    std::cout << "\n@" << winner_name << " is the winner! :)\n";
    std::flush(std::cout);
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
#ifdef LAND_NUMBER
        int size_ = this->map.get_size();
        int width_ = 0;
        while (size_) {
            size_ /= 10;
            ++width_;
        }
        std::cout << "[" << std::right << std::setw(width_) << i << "]";
#endif
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