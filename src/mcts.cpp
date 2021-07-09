#include "mcts.hpp"

#include "node.hpp"

void MCTS::best_move(HexState& state) {
    int simulations = 1000;
    rootNode = Node(state);
    for (int i = 0; i < simulations; i++) {
        Node node = rootNode.select();
        node.expand(board);
        for (Node child : node.children) {
            child.backpropagate(child.randomPlayout(board));
        }
    }
}