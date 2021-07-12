// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.cpp
#include "node.hpp"

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
            // simulate(edge)
        }
    }
}

void Node::backpropagate(Player winner) {
    record_game(winner == state->currentPlayer);
    if (parent != NULL) {
        parent->backpropagate(winner);
    }
}

Player Node::randomPlayout(HexBoard& board) {
    HexState* copyState = new HexState();
    copyState->copy_from(*state);
    while (true) {
        int move = rand() % BOARD_SIZE;
        if (board.move(*copyState, move)) {
            break;
        }
    }
    Player winner = state->currentPlayer;
    delete copyState;
    return winner;
}

void Node::record_game(bool win) {
    wins += win;
    games++;
}

float Node::ucb_eval(Node& child) {
    if (games == 0) {
        return 1e10;  // "infinity"
    }
    return (1.0 - float(child.wins) / child.games) + UCB_EXPLORE * sqrt(sqrt(games) / child.games);
}
