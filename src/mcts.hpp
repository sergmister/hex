#include "node.hpp"

struct MCTS {
    HexState rootState;

    Node rootNode;
    HexBoard board;

    void best_move(HexState& state);
    float ucb_eval(Node& child);  // "upper confidence bound" (used to determine which node to explore next)

    // int uct_playout(HexState& board, int player, bool useMiai);  // uct is just a name
};