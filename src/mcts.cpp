#include "mcts.hpp"

#include "node.hpp"

int MCTS::best_move(HexState& state) {
    int simulations = 1000;
    Node rootNode(&state);
    for (int i = 0; i < simulations; i++) {
        Node* node = rootNode.select();
        node->expand(board);
        for (Node* child : node->children) {
            Player winner = child->randomPlayout(board);
            child->backpropagate(winner);
        }
    }
    int most_trials = -1;
    Node* best_child;
    for (Node* child : rootNode.children) {
        if (child->games > most_trials) {
            most_trials = child->games;
            best_child = child;
        }
    }
    return best_child->move_number;
}