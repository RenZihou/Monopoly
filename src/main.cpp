// main.cpp
// -*- encoding: utf-8 -*-
// @Author: RZH

#include <string>
#include <iostream>
#include <vector>
#include <random>
#include "game.h"

int main() {
    system("clear");
#ifdef CHEAT_ON
    std::cout << "[Notice] Cheat mode is on!\n";
#endif
    std::cout << "==== welcome to MONOPOLY GAME ====\n"
              << "====  developed by Ren Zihou  ====\n\n";
    while (true) {
        std::cout << "type <g> to start game, <h> for help, <q> to quit\n";
        int key = Game::keyboard();
        std::cout << "\n";
        if (key == 'g') {
            // start game
            // settings
            int player_number;
            std::vector<std::string> player_names;
            std::cout << "\n==== Settings ====\nPlease input player number: ";
            std::cin >> player_number;
            for (int i = 1; i <= player_number; ++i) {
                std::string name;
                std::cout << "Please input Player" << i << "'s name: ";
                std::cin >> name;
                player_names.push_back(name);
            }
            std::string seed_s;
            int seed;
            std::cout << "Please input world seed (no input will use a random seed): ";
//            std::cin >> seed_s;
            std::cin.ignore();
            getline(std::cin, seed_s, '\n');
            if (seed_s.empty()) {
                // use random seed
                std::random_device rd{};
                seed = static_cast<int>(rd());
            } else {
                seed = std::stoi(seed_s);
            }
            std::string size_s;
            int size;
            std::cout << "Please input world size (no input will use default size): ";
            getline(std::cin, size_s, '\n');
            size = size_s.empty()
                   ? static_cast<int>(Config::config().get_world_rule("default_size"))
                   : static_cast<int>(std::stoi(size_s));
            Game::game().setup(size, seed, player_names);
            Game::game().run();
        } else if (key == 'h') {
            // TODO: print manual
        } else if (key == 'q') {
            break;
        }
    }
    return 0;
}
