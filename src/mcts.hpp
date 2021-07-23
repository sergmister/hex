#include "node.hpp"

struct MCTS {
    MCTS(HexState* state);
    ~MCTS();
    HexState* rootState;
    Node* rootNode;
    HexBoard board;
    int best_move();
    int best_move(int simulations);
    void move(int i);
};