#pragma once
#include "game.hpp"

struct Arena {
    int win1 = 0, win2 = 0;
    void play_games(int amount);
    void play_game(bool first);
    void print();
};