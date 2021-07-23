// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.cpp
#include "node.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>

#include "game.hpp"
#include "math.h"
#include "mcts.hpp"

Node::Node() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        bridge[i] = -1;
    }
}
Node::~Node() {
    for (int i = 0; i < children.size(); i++) {
        delete children[i]->state;
        delete children[i];
    }
}

Node::Node(HexState* state) : Node() {
    this->state = state;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (state->board[i] == CellState::Empty) {
            child_moves.push_back(i);
        }
    }
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(child_moves), std::end(child_moves), rng);
}

Node* Node::select() {
    Node* best = this;
    while (!best->isLeaf() && !best->state->gameOver && best->children.size() == best->child_moves.size()) {
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

inline int to_index(int r, int c) { return r * BOARD_WIDTH + c; }
inline int to_index(std::pair<int, int> p) { return to_index(p.first, p.second); }
inline bool in_bound(int r, int c) { return r >= 0 && r < BOARD_HEIGHT && c >= 0 && c < BOARD_WIDTH; }
inline bool in_bound(std::pair<int, int> p) { return in_bound(p.first, p.second); }

void Node::update_bridges(int move) {
    int r = move / BOARD_WIDTH;
    int c = move % BOARD_WIDTH;
    std::vector<std::pair<int, int>> neighbors = {{r + 1, c}, {r + 1, c - 1}, {r, c - 1},
                                                  {r - 1, c}, {r - 1, c + 1}, {r, c + 1}};
    for (int i = 0; i < neighbors.size(); i++) {
        if (!in_bound(neighbors[i])) {
            neighbors.erase(neighbors.begin() + i);
            i--;
        }
    }
    // store pairs of adjecent cells
    std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> pairs = {};
    for (int i = 0; i < neighbors.size(); i++) {
        pairs.push_back({neighbors[i], neighbors[(i + 1) % neighbors.size()]});
    }
    // shuffle pairs of neighbors in a random order;
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(pairs), std::end(pairs), rng);

    for (int i = 0; i < neighbors.size(); i++) {
        std::pair<int, int> cell1 = pairs[i].first, cell2 = pairs[i].second;
    }
}

void Node::expand(HexBoard& board) {
    if (state->gameOver || children.size() == child_moves.size()) {
        return;
    }
    int move = child_moves[children.size()];
    HexState* child_state = new HexState();
    child_state->copy_from(*state);
    board.move(*child_state, move);

    Node* child = new Node(child_state);
    for (int j = 0; j < BOARD_SIZE; j++) {
        child->bridge[j] = bridge[j];
    }
    child->move_number = move;
    child->parent = this;
    children.push_back(child);
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
