#include "node.hpp"

struct MCTS {
    HexState rootState;

    Node rootNode;
    HexBoard board;

    int best_move(HexState& state);
};