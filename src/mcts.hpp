#include "node.hpp"

struct MCTS {
    HexState rootState;

    Node rootNode;
    HexBoard board;

    int best_move(HexState& state);

    // int uct_playout(HexState& board, int player, bool useMiai);  // uct is just a name
};