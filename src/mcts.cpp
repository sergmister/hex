#include "mcts.hpp"

#include "node.hpp"

MCTS::MCTS(HexState* state) {
    rootState = state;
    rootNode = new Node(state);
}
MCTS::~MCTS() { delete rootNode; }
void MCTS::move(int i) {
    for (Node* child : rootNode->children) {
        if (child->move_number == i) {
            rootNode = child;
            return;
        }
    }
}
int MCTS::best_move() { return best_move(100); }
int MCTS::best_move(int simulations) {
    for (int i = 0; i < simulations; i++) {
        Node* node = rootNode->select();
        node->expand(board);
        if (node->isLeaf()) {
            Player winner = node->randomPlayout(board);
            node->backpropagate(winner);
        }
        for (Node* child : node->children) {
            Player winner = child->randomPlayout(board);
            child->backpropagate(winner);
        }
    }
    int most_trials = -1;
    Node* best_child;
    for (Node* child : rootNode->children) {
        if (child->games > most_trials) {
            most_trials = child->games;
            best_child = child;
        }
    }
    return best_child->move_number;
}