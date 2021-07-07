// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.cpp
#include "mcts.hpp"
#include "math.h"

void Node::expand(HexState& board) {}

int Node::bestChildIndex() { return 0; }

int Node::uct_playout(HexState& board, int player, bool useMiai) {
    if (isLeaf()) {
        if (games >= EXPAND_THRESHOLD) {
            expand(board);
        }
    }
    int winner;
    if (isLeaf()) {
        // PLAY GAME TO FIND WINNER
    } else {
        Node& c = children[bestChildIndex()];
        // todo
    }
    games++;
    if (winner == player) {
        wins++;
    }
    return winner;
}

float Node::ucb_eval(Node& child) { 
    if (games == 0) {
        return 1e10; // "infinity"
    }
    return (1.0 - float(child.wins)/child.games) + UCB_EXPLORE*sqrt(sqrt(games)/child.games);
}