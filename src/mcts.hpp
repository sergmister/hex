#include "node.hpp"

struct MCTS {
    MCTS(HexState* state);
    HexState* rootState;
    Node* rootNode;
    HexBoard board;

    int best_move(HexState& state);
    int best_move(HexState& state, int simulations);
    void move(int i);
};