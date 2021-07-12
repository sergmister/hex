// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.h
#pragma once
#include <vector>

#include "game.hpp"
static const int UNKNOWN = 2;
static const int PROVEN_WIN = 1;
static const int PROVEN_LOSS = 0;

static const float UCB_EXPLORE = 1.0;
static const int EXPAND_THRESHOLD = 2;

struct Node {
    int wins = 0;
    int games = 0;
    int move_number = 0;
    int proofStatus;  // Whether it is unkown, proven win, or proven loss

    Node* parent = NULL;
    std::vector<Node*> children;

    Node();
    ~Node();
    // Node(const Node& node);
    Node(HexState* state);
    HexState* state;
    void record_game(bool win);  // update number of wins and games

    void backpropagate(Player winner);
    Node* select();  // selection
    void expand(HexBoard& board);
    Player randomPlayout(HexBoard& board);  // find winner of random game (simulate)

    int bestChildIndex();
    bool isLeaf() {
        return children.empty();  // If no children, then it is a leaf
    }
    float ucb_eval(Node& child);  // "upper confidence bound" (used to determine which node to explore next)
};