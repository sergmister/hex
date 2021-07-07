// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.h
#include <vector>

#include "game.hpp"
static const int UNKNOWN = 2;
static const int PROVEN_WIN = 1;
static const int PROVEN_LOSS = 0;

static const float UCB_EXPLORE = 1.0;
static const int EXPAND_THRESHOLD = 2;

struct Edge;

struct Node {
    int wins;
    int games;
    int proofStatus;  // Whether it is unkown, proven win, or proven loss

    Node* parent;
    std::vector<Edge> edges;

    Node() {
        proofStatus = UNKNOWN;
        wins = 0;
        games = 0;
    }

    void record_game(bool win);  // update number of wins and games
    void expand(HexState& board);
    int bestMove(HexState& board, int plays);
    Player randomPlayout(HexState state);  // find winner of random game
    int bestChildIndex();
    bool isLeaf() {
        return edges.empty();  // If no children, then it is a leaf
    }
    float ucb_eval(Node& child);  // "upper confidence bound" (used to determine which node to explore next)
    int uct_playout(HexState& board, int player, bool useMiai);  // uct is just a name
    void record_game(bool win);  // update number of wins and games
};

struct Edge {
    uint16_t move;
    Node child;
};

struct MCTS {
    HexState rootState;

    Node rootNode;
    HexBoard board;


    Node select();  // selection

    void expand_and_evaluate(HexState& board);

    void backpropagate();

    // int bestMove(HexState& board, int plays);

    Player randomPlayout();  // find winner of random game

    float ucb_eval(Node& child);  // "upper confidence bound" (used to determine which node to explore next)

    // int uct_playout(HexState& board, int player, bool useMiai);  // uct is just a name
}