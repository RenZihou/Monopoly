# Monopoly

[TOC]

***

## Game Rules

### Basic Rules

* Initial Funds: 1500 / player
* Rules of Moving: roll / strategy card
* Commercial Land:
    * empty: buy with fund
    * self: upgrade
    * other's: pay rent (oblige)
* Functional Land:
    * providing strategy card
    * ban moving for 3 rounds
* Strategy Card:
    * 1 ~ 5 step card
* Transfer & Promotion:
    * Skills
        * Effects
        * Cooldown time
        * level
    * Promotion
    * Transfer
* Loss & Win:
    * Out if fund <= 0
    * Once a player is broke, his / her buildings goes vacant while whose levels remains unchanged
* Other Rules:
    * one player on each land at most (force previous player to move forward 1 step)
    * each player will receive a small amount of fund at each of his / her round. The more buildings he / she owns, the
      more money he / she receives.

### About Strategy Cards

The only legal way to obtain a strategy card (*card for short*) is through reaching a functional land.

Each functional land gives a certain type of card. These cards have various effects and use conditions.

A passive card can only be activated when certain condition is triggerred.

***

## Project structure

### Game

singleton

* each round (each player):

1. check (update) `movable` -> jump to 2 or 4
2. roll or use card
3. update `pos`
4. check `land`
5.

```c++
class Game {
private:
// several functions to manipulate
void exec(std::vector<std::string>);
};
```

### Player

```c++
class Player {
string name;
int fund;
int spwan_pos;
int pos;
int xp;
vector<Building> buildings;
vector<Card> cards;
Skill skill;
};
```

### World

#### Land

```c++
#define
EMPTY 0
#define
COMMERCIAL 1
#define
FUNCTIONAL 2

class Land {
// int type;
};

class CommercialLand : Land {
int owner;
Building building;
int level;
};

class FunctionalLand : Land {
int function;
};
```

#### Map

```c++
class Map {
int size;
vector<Land> map;
};
```

#### Building

```c++
class Building {
string name;
int max_level;
vector<int> upgrade_costs;
vector<int> rents;
}
```



### Card

### Skill


### Configuration

use config file (``.json`)

#### Strategy Card

```json
[
  {
    "name": "1-step move",
    "description": "move one step forward and skip the roll",
    "effect": [],
    "condition": [],
    "weight": 0.1
  }
]
```

#### Skill

```json
[
  {
    "name": "roll twice",
    "description": "",
    "maxLv": 3,
    "effect": [],
    "coolDown": [],
    "weight": 0.1
  }
]
```

***

## Functions

* `move <player_id> <steps>`
* `moveto <player_id> <target>`

***

## Game Running

### A Cycle

A cycle is a basic unit of the game. In each cycle, only one players would engage in the game (although this might
change other players' propeties).

Here is what the game would do in a cycle:

1. print the map, the current player's info (ie. fund, skill, available cards)
2. waiting for player's decision: get a roll or use a card
3. roll / using card and update player's position or fund (this may clear the console)
4. do land functions (pay fund or buy land or give card)

At the beginning of each cycle, the console would be cleared.

### A Round

A round is a sequence of cycles, during which each player plays a cycle (expect for skipping)
