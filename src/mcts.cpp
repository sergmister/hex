// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.cpp
#include "mcts.hpp"
#include "game.hpp"
#include "math.h"

Node Node::select() {
    Node best = *this;
    while (!best.isLeaf() && !best.state.gameOver) {
        float best_eval = -1;
        Node best_child;
        for (Edge edge : best.edges) {
            float eval = best.ucb_eval(edge.child);
            if (eval > best_eval) {
                best_eval = eval;
                best_child = edge.child;
            }
        }
        best = best_child;
    }
    return best;
}
void Node::expand(HexBoard& board) {
    int empty = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (state.board[i] == CellState::Empty) {
            HexState child_state = HexState(state);
            board.move(child_state, i);
            Node child(child_state);
            Edge edge(child);
            edge.child = child;
            edge.move = i;
            child.parent = this;
            edges.push_back(edge);
            // simulate(edge)
        }
    }
    
}

void Node::backpropagate(Player winner) {
    record_game(winner == state.currentPlayer);
    if (parent) {
        parent->backpropagate(winner);
    }
}

Player Node::randomPlayout(HexBoard& board) {
    while (true) {
        int move = rand() % BOARD_SIZE;
        if (board.move(state, move)) {
            break;
        }
    }
    return state.currentPlayer;
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

void MCTS::best_move(HexState& state) {
    int simulations = 1000;
    rootNode = Node(state);
    for (int i = 0; i < simulations; i++) {
        Node node = rootNode.select();
        // Node child;
        //TODO
    }
}