// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.h
#include "game.hpp"
#include <vector>
static const int UNKNOWN = 2;
static const int PROVEN_WIN = 1;
static const int PROVEN_LOSS = 0;

static const float UCB_EXPLORE = 1.0;
static const int EXPAND_THRESHOLD = 2;

struct Node {
    int wins;
    int games;
    int proofStatus;  // Whether it is unkown, proven win, or proven loss
    int lcn;
    std::vector<Node> children;
    Node() {
        proofStatus = UNKNOWN;
        wins = 0;
        games = 0;
    }

    void expand(HexState& board);
    int bestMove();
    int bestChildIndex();
    bool isLeaf() {
        return !children.size();  // If no children, then it is a leaf
    }
    float ucb_eval(
        Node& child);  // "upper confidence bound" (used to determine which node to explore next)
    int uct_playout(HexState& board, int player, bool useMiai);  // uct is just a name
};