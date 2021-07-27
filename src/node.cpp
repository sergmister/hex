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
        bridge_white[i] = -1;
        bridge_black[i] = -1;
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
Node::Node(HexState* state, int only_child) : Node() {
    this->state = state;
    child_moves.push_back(only_child);
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
inline void print(std::pair<int, int> p) {}

CellState Node::get_cell(std::pair<int, int> p) {
    CellState cellState;
    if (p.first < 0 || p.first >= BOARD_HEIGHT) {
        cellState = CellState::Black;
    } else if (p.second < 0 || p.second >= BOARD_WIDTH) {
        cellState = CellState::White;
    } else {
        cellState = state->board[to_index(p)];
    }
    return cellState;
}

void Node::update_bridges(int move) {
    int r = move / BOARD_WIDTH;
    int c = move % BOARD_WIDTH;
    std::vector<std::pair<int, int>> neighbors = {{r + 1, c}, {r + 1, c - 1}, {r, c - 1},
                                                  {r - 1, c}, {r - 1, c + 1}, {r, c + 1}};
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
        if (!in_bound(cell1) || !in_bound(cell2)) continue;
        if (state->board[to_index(cell1)] != CellState::Empty || state->board[to_index(cell2)] != CellState::Empty) {
            continue;
        }
        std::pair<int, int> bridge1, bridge2;
        // find the two cells where the bridge connects to
        if (cell1.first == cell2.first) {
            bridge1 = {cell1.first - 1, std::max(cell1.second, cell2.second)};
            bridge2 = {cell1.first + 1, std::min(cell1.second, cell2.second)};
        } else if (cell1.second == cell2.second) {
            bridge1 = {std::max(cell1.first, cell2.first), cell1.second - 1};
            bridge2 = {std::min(cell1.first, cell2.first), cell1.second + 1};
        } else {
            bridge1 = {std::max(cell1.first, cell2.first), std::max(cell1.second, cell2.second)};
            bridge2 = {std::min(cell1.first, cell2.first), std::min(cell1.second, cell2.second)};
        }
        if (bridge1.first == r && bridge1.second == c) {
            swap(bridge1, bridge2);
        }
        // get cellstates of the two connecting bridge cells
        int cell_state1 = (int)get_cell(bridge1), cell_state2 = (int)get_cell(bridge2);
        int* bridge;
        // if cells are same color
        if (cell_state1 * cell_state2 > 0) {
            // if cell is white
            if (cell_state1 < 0) {
                bridge = bridge_white;
            } else {
                bridge = bridge_black;
            }
            if (bridge[to_index(cell1)] == -1 && bridge[to_index(cell2)] == -1) {
                bridge[to_index(cell1)] = to_index(cell2);
                bridge[to_index(cell2)] = to_index(cell1);
            }
        }
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
    Node* child;
    if (state->currentPlayer == Player::Black && bridge_white[move] != -1 &&
        state->board[bridge_white[move]] == CellState::Empty) {
        child = new Node(child_state, bridge_white[move]);
    } else if (state->currentPlayer == Player::White && bridge_black[move] != -1 &&
               state->board[bridge_black[move]] == CellState::Empty) {
        child = new Node(child_state, bridge_black[move]);
    } else {
        child = new Node(child_state);
    }
    for (int j = 0; j < BOARD_SIZE; j++) {
        child->bridge_white[j] = bridge_white[j];
        child->bridge_black[j] = bridge_black[j];
    }
    child->update_bridges(move);
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
