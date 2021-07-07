#include "mcts.hpp"

void Node::expand(HexState& board) {}

int Node::bestChildIndex() { return 0; }

int Node::uct_playout(HexState& board, int player, bool useMiai) {
    if (isLeaf()) {
        if (games >= EXPAND_THRESHOLD) {
            expand(board);
        }
    }
    int winner;
    if (isLeaf()) {
        // PLAY GAME TO FIND WINNER
    } else {
        Node& c = children[bestChildIndex()];
        // todo
    }
    games++;
    if (winner == player) {
        wins++;
    }
    return winner;
}