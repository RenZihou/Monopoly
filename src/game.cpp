// game.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include "game.h"

#include <iostream>
#include <iomanip>
#include <random>
#include <vector>
#include <exception>
#include <termio.h>
#include <cstdio>
#include <cstdlib>
#include <string>

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
    auto land = dynamic_cast<CLand *>(this->map[pos]);
    land->set_owner(player_id, this->players[player_id]->get_name());
    return true;
}

bool Game::_setfund(int player_id, int new_fund) {
    Player *player = this->players[player_id];
    int add = new_fund - player->get_fund();
    player->upd_fund(add);
    return true;
}

bool Game::_addfund(int player_id, int add) {
    this->players[player_id]->upd_fund(add);
    return true;
}

bool Game::_freeze(int player_id, int round) {
    this->players[player_id]->freeze(round);
    return true;
}

bool Game::_kill(int player_id) {
    this->_setfund(player_id, 0);
    return true;
}

bool Game::_upgrade(int pos) {
    auto land = dynamic_cast<CLand *>(this->map[pos]);
    land->upgrade();
    return true;
}

bool Game::_seed() {
    std::cout << "[seed] " << this->map.get_seed() << "\n";
    return true;
}

bool Game::use_card(Player *player, const std::string &condition) {
    std::vector<Card *> available;
    for (auto &card : player->get_cards()) {
        if (card->has_condition(condition)) available.push_back(card);
    }
    // list available cards
    bool used = true;
    if (!available.empty()) {
        Game::display_cards(available);
        while (true) {
            std::cout << "Choose a card to use, type <x> to cancel\n";
            int key_ = Game::keyboard();
            if (key_ == 'x') {
                used = false;
                break;
            } else if (key_ >= '0' && key_ < '0' + available.size()) {
                this->exec(available[key_ - '0']->get_effect());
                player->remove_card(available[key_ - '0']);
                break;
            }
        }
    } else {
        used = false;
        std::cout << "No available cards :(\n";
    }
    return used;
}

bool Game::use_skill(Player *player, const std::string &condition) {
    if (player->can_use_skill(condition)) {
        // use skill
        this->exec(player->use_skill());
        return true;
    } else {
        std::cout << "You cannot use your skill yet :(\n";
        return false;
    }
}

bool Game::exec(std::string command) {
    std::vector<std::string> cmd;
    std::stringstream commandss(command);
    while (commandss >> command) cmd.push_back(command);
    auto it = cmd.begin();
    std::string name = *it;
    if (name == "move") {
        std::string pid = *(++it);
        int player_id = pid == "~" ? this->context.curr_player : std::stoi(pid);
        int steps = std::stoi(*(++it));
        return this->_move(player_id, steps);
    } else if (name == "moveto") {
        std::string pid = *(++it);
        int player_id = pid == "~" ? this->context.curr_player : std::stoi(pid);
        int target = std::stoi(*(++it));
        return this->_moveto(player_id, target);
    } else if (name == "setfund") {
        std::string pid = *(++it);
        int player_id = pid == "~" ? this->context.curr_player : std::stoi(pid);
        int fund = std::stoi(*(++it));
        return this->_setfund(player_id, fund);
    } else if (name == "setowner") {
        std::string p = *(++it);
        int pos = p == "~" ? this->context.curr_land : std::stoi(p);
        std::string pid = *(++it);
        int player_id = pid == "~" ? this->context.curr_player : std::stoi(pid);
        return this->_setowner(pos, player_id);
    } else if (name == "addfund") {
        std::string pid = *(++it);
        int player_id = pid == "~" ? this->context.curr_player : std::stoi(pid);
        int add = std::stoi(*(++it));
        return this->_addfund(player_id, add);
    } else if (name == "freeze") {
        std::string pid = *(++it);
        int player_id = pid == "~" ? this->context.curr_player : std::stoi(pid);
        int round = std::stoi(*(++it));
        return this->_freeze(player_id, round);
    } else if (name == "kill") {
        std::string pid = *(++it);
        int player_id = pid == "~" ? this->context.curr_player : std::stoi(pid);
        return this->_kill(player_id);
    } else if (name == "upgrade") {
        std::string p = *(++it);
        int pos = p == "~" ? this->context.curr_land : std::stoi(p);
        return this->_upgrade(pos);
    } else if (name == "seed") {
        return this->_seed();
    }
    return false;
}

bool Game::exec(const std::vector<std::string> &cmd) {
    for (const auto &cmd_ : cmd) this->exec(cmd_);
    return true;
}

void Game::roll() {
    Player *player = this->players[this->context.curr_player];
    while (true) {
        std::cout << "Make Your Decision! "
                     "(type <r> for roll, <c> for card, "
                     "<s> for skill, <h> for help)\n";
        int key = Game::keyboard();
        if (key == 'r') {  // roll to move
            std::random_device rd{};
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> roll(1, 6);
            int steps = roll(mt);
            this->_move(player->get_id(), steps);
            break;
        } else if (key == 'c') {
            std::cout << "\n";
            if (this->use_card(player, "roll")) break;
        } else if (key == 's') {
            std::cout << "\n";
            this->use_skill(player, "roll");
        } else if (key == 'h') {
            Game::manual();
            this->display(player->get_id());
        }
#ifdef CHEAT_ON  // only available when CHEAT_ON flag is on
        else if (key == '0') {  // cheat mode
            while (true) {
                std::cout << "> ";
                std::string command;
                getline(std::cin, command);
                if (command == "quit") break;
                else this->exec(command);
            }
            break;
        }
#endif  // CHEAT_ON
    }
}

void Game::buy() {
    Player *player = this->players[this->context.curr_player];
    auto curr_land = dynamic_cast<CLand *>(this->map[this->context.curr_land]);
    while (true) {
        std::cout << "You reached a vacant commercial land!"
                     "(type <b> to buy it, <s> to use skill, "
                     "<h> for help, <p> to do nothing)\n";
        int key = Game::keyboard();
        if (key == 'b') {
            if (player->buy_land(curr_land)) {
                std::cout << "You bought it successfully!\n";
            } else {
                std::cout << "You have no enough money :(\n";
            }
            break;
        } else if (key == 's') {
            std::cout << "\n";
            if (this->use_skill(player, "buy")) break;
        } else if (key == 'h') {
            Game::manual();
            this->display(player->get_id());
        } else if (key == 'p') break;
    }
}

void Game::upgrade() {
    Player *player = this->players[this->context.curr_player];
    auto curr_land = dynamic_cast<CLand *>(this->map[this->context.curr_land]);
    while (true) {
        std::cout << "You reached your own land!"
                     "(type <u> to upgrade your building, <s> to use skill, "
                     "<h> for help, <p> to do nothing\n";
        int key = Game::keyboard();
        if (key == 'u') {
            if (player->upgrade_land(curr_land)) {
                std::cout << "You upgraded it successfully!\n";
            } else {
                std::cout << "You have no enough money :(\n";
            }
            break;
        } else if (key == 's') {
            if (this->use_skill(player, "upgrade")) break;
        } else if (key == 'h') {
            Game::manual();
            this->display(player->get_id());
        } else if (key == 'p') break;
    }
}

void Game::rent() {
    Player *player = this->players[this->context.curr_player];
    auto curr_land = dynamic_cast<CLand *>(this->map[this->context.curr_land]);
    int rent = curr_land->get_rent();
    std::cout << "You reached other's land and should pay $" << rent << "\n";
    if (!this->use_card(player, "rent")) {
        this->players[curr_land->get_owner()]->upd_fund(rent);
        player->upd_fund(-rent);
    }
}

void Game::setup(int world_size, int seed,
                 const std::vector<std::string> &player_names) {
    // generate map
    if (world_size < player_names.size())
        throw std::invalid_argument(
                "world size cannot be smaller than player number");
    double CLand_prob = this->config->get_world_rule("CLand_prob");
    double FLand_prob = this->config->get_world_rule("FLand_prob");
    this->map = std::move(Map(world_size, seed, CLand_prob, FLand_prob));

    // spawn players
    this->alive = static_cast<int>(player_names.size());
    int init_fund = static_cast<int>(this->config->get_player_rule(
            "init_fund"));
    int player_id = 0;
    std::mt19937 mt(seed);
    std::uniform_int_distribution<int> pos(0, world_size - 1);
    for (auto &name : player_names) {
        auto *new_player = new Player(player_id, name, pos(mt), init_fund);
        this->players.push_back(new_player);
        this->_move(player_id, 0);
        ++player_id;
    }
}

Game *Game::cycle(int player_id) {
    Player *player = this->players[player_id];
    this->context.curr_player = player_id;
    this->context.curr_land = player->get_position();
    player->cool_down();
    // print the map and player info
    this->display(player_id);

    // player instruction
    this->roll();

    this->context.curr_land = player->get_position();
    this->display(player_id);

    // skill promote & transfer
    if (player->at_spawn_pos()) {
        std::cout << "You reached your spawn land!\n"
                  << "You can now choose to promote your skill (<u>) or "
                  << "to transfer to another (<t>)\n";
        while (true) {
            int key = Game::keyboard();
            if (key == 'u') {
                // promote skill
                if (player->promote()) {
                    std::cout << "\nYou have promoted your skill successfully!\n";
                } else {
                    std::cout << "\nYou are already at the highest level!\n";
                }
                break;
            } else if (key == 't') {
                // transfer
                std::string curr_name = player->get_skill_name();
                while (true) {
                    Skill *new_skill = SkillFactory::factory().gen_skill();
                    if (new_skill->get_name() != curr_name) {
                        player->transfer(new_skill);
                        break;
                    }
                    delete new_skill;
                }
                break;
            }
        }
        this->display(player_id);
    }

    // check new land
    Land *curr_land_ = this->map[this->context.curr_land];
    if (curr_land_->get_type() == COMMERCIAL) {
        auto *curr_land = dynamic_cast<CLand *>(curr_land_);
        if (curr_land->get_owner() == -1) {
            // vacant land, buy or not
            this->buy();
        } else if (curr_land->get_owner() == player_id) {
            // player's land, upgrade or not
            this->upgrade();
        } else {
            // other's land, pay rent
            this->rent();
        }
    } else if (curr_land_->get_type() == FUNCTIONAL) {
        auto curr_land = dynamic_cast<FLand *>(curr_land_);
        Card *card = curr_land->get_card();
        if (card->has_condition("instant")) {
            for (const auto &command : card->get_effect()) this->exec(command);
        } else {
            player->add_card(card);
        }
    }
    return this;
}

Game *Game::run() {
    int player_id = 0;
    int tot_player = static_cast<int>(this->players.size());
    while (this->alive > 1) {
        if (this->players[player_id] != nullptr) {
            if (!this->players[player_id]->freeze()) this->cycle(player_id);
            else this->players[player_id]->freeze(-1);
        }

        for (int pid = 0; pid < tot_player; ++pid) {
            Player *player = this->players[pid];
            bool revive_flag = false;
            if (player != nullptr && player->get_fund() < 0) {
                // check for revive card
                this->context.curr_player = pid;
                this->context.curr_land = player->get_position();
                for (auto &card : player->get_cards()) {
                    if (card->has_condition("broke")) {
                        for (const auto &effect : card->get_effect())
                            if (effect == "revive") {
                                revive_flag = true;
                                if (player->get_fund() < 0) this->exec("setfund ~ 0");
                            } else this->exec(effect);
                        player->remove_card(card);
                    }
                }
                if (revive_flag) continue;
                // TODO: display opt
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
        int i_width = 0;
        while (size_) {
            size_ /= 10;
            ++i_width;
        }
        std::cout << "[" << std::right << std::setw(i_width) << i << "]";
#endif  // LAND_NUMBER
        std::cout << "[ " << std::left << std::setw(d_width) << descriptions[i]
                  << " ]";
        if (this->players_pos.count(i))  // has player at this position
            std::cout << " @"
                      << this->players[this->players_pos[i]]->get_name()
                      << " ($"
                      << this->players[this->players_pos[i]]->get_fund()
                      << ")";
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
    std::cout << "Your Skill: " << this->players[player_id]->get_skill_des() << "\n";
    std::flush(std::cout);
}

void Game::display_cards(std::vector<Card *> cards) {
    int i_width = 5;
    int n_width = 4;
    int d_width = 11;
    for (const auto &card : cards) {
        int n_len = static_cast<int>(card->get_name().size());
        int d_len = static_cast<int>(card->get_des().size());
        n_width = n_width > n_len ? n_width : n_len;
        d_width = d_width > d_len ? d_width : d_len;
    }

    std::cout << "\nINDEX" << " | " << std::setw(n_width) << "NAME" << " | "
              << "DESCRIPTION\n";
    std::cout.fill('-');
    std::cout << std::setw(i_width) << "" << " | "
              << std::setw(n_width) << "" << " | "
              << std::setw(d_width) << "" << "\n";
    std::cout.fill(' ');

    int size = static_cast<int>(cards.size());
    for (int i = 0; i < size; ++i) {
        std::cout << std::setw(i_width) << i << " | "
                  << std::setw(n_width) << cards[i]->get_name() << " | "
                  << std::setw(d_width) << cards[i]->get_des() << "\n";
    }
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

void Game::manual() {
    system("clear");
    std::cout << "==== MANUAL ====\n";
    std::cout << "RULES\n"
                 "The basic goal of this game is to buy lands, upgrade your "
                 "buildings and make others pay rents and finally go broke. "
                 "If you are the only survivor, then you win the game.\n\n"
                 "Each round of game (per player) is divided into 3 stages:\n"
                 "  Roll: at this stage, player can choose to roll to move, "
                 "use cards or use their skills.\n"
                 "  Upskill: this stage will only be triggered if player "
                 "arrives ar his/her spawn land. player can choose to promote "
                 "their skills or transfer to another skill.\n"
                 "  Buy/Upgrade/Rent: player goes into this stage if he/she "
                 "arrives at a commercial land, and depend on the owner of "
                 "the land, the player would take different actions.\n"
                 "    Buy: if that is a vacant commercial land, player can "
                 "pay to buy the house on which.\n"
                 "    Upgrade: if the land belongs to the player, he/she "
                 "can pay to upgrade the building to make more rent.\n"
                 "    Rent: if the land belongs to others, the player should "
                 "pay certain amount of rent to the owner.\n\n"
                 "CARDS\n"
                 "Cards are given to player when he/she arrives at a "
                 "functional land. Cards has various effects and trigger "
                 "conditions.\n"
                 "You can type <c> to use your cards (if available).\n\n"
                 "SKILLS\n"
                 "Players obtain random lv.0 skills at the beginning. Skills "
                 "requires several rounds to cool down before another "
                 "application.\n"
                 "Once a player arrives at his/her spawn land, he/she can "
                 "choose to promote the skill or transfer to another skill. "
                 "Promoting will strengthen the effect or short down the "
                 "cool-down time. Transferring will give you another lv.0 "
                 "random skill and disable your previous skill.\n\n";
    std::cout << "press any key to exit.\n";
    Game::keyboard();
    system("clear");
}