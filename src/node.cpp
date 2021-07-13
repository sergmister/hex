// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.cpp
#include "node.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "game.hpp"
#include "math.h"
#include "mcts.hpp"

Node::Node() {}
Node::~Node() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i]->state;
        delete children[i];
    }
}

// Node::Node(const Node& node) {
//     wins = node.games;
//     games = node.games;
//     parent = node.parent;
//     move_number = node.move_number;
// }

Node::Node(HexState* state) { this->state = state; }

Node* Node::select() {
    Node* best = this;
    while (!best->isLeaf() && !best->state->gameOver) {
        float best_eval = -1;
        Node* best_child;
        for (Node* child : best->children) {
            float eval = best->ucb_eval(*child);
            if (eval > best_eval) {
                best_eval = eval;
                best_child = child;
            }
        }
        best = best_child;
    }
    return best;
}
void Node::expand(HexBoard& board) {
    if (!isLeaf() || state->gameOver) {
        return;
    }
    int empty = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (state->board[i] == CellState::Empty) {
            HexState* child_state = new HexState();
            child_state->copy_from(*state);
            board.move(*child_state, i);

            Node* child = new Node(child_state);
            child->move_number = i;
            child->parent = this;
            children.push_back(child);
        }
    }
}

void Node::backpropagate(Player winner) {
    games++;
    if (parent) {
        if (winner == parent->state->currentPlayer) {
            wins++;
        }
        parent->backpropagate(winner);
    }
}

Player Node::randomPlayout(HexBoard& board) {
    if (state->gameOver) {
        return state->currentPlayer;
    }
    HexState* copyState = new HexState();
    copyState->copy_from(*state);

    std::vector<int> moves;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (copyState->board[i] == CellState::Empty) {
            moves.push_back(i);
        }
    }
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(moves), std::end(moves), rng);

    for (int i = 0; i < moves.size(); i++) {
        if (board.move(*copyState, moves[i])) {
            break;
        }
    }
    Player winner = copyState->currentPlayer;
    delete copyState;
    return winner;
}

float Node::ucb_eval(Node& child) {
    if (games == 0) {
        return 1e10;  // "infinity"
    }
    return ((float(child.wins) / child.games)) + UCB_EXPLORE * sqrt(sqrt(games) / child.games);
}
