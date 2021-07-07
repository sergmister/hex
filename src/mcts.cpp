// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.cpp
#include "mcts.hpp"

#include "game.hpp"
#include "math.h"

Node MCTS::select() {
    Node best = this;
    while (!best.isLeaf()) {// todo: add game over condition
        float best_eval = -1;
        Node best_child;
        for (Edge edge : best.edges) {
            float eval = best.ucb_eval(edge.child);
            if (eval > best_eval) {
                best_eval = eval;
                best_child = edge.child;
            }
        }
        best = best_child
    }
    return best;
}
void Node::expand(HexState& state) {
    int empty = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (state.board[i] == CellState::Empty) {
            Edge edge;
            Node child;
            edge.child = child;
            edge.move = i;
            edges.push_back(edge);
            // simulate(edge)
        }
    }
    
}

Player MCTS::randomPlayout() {
    while (true) {
        int move = rand() % BOARD_SIZE;
        if (board.move(rootState, move)) {
            break;
        }
    }
    return rootState.currentPlayer;
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