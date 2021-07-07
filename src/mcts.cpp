// Taken from https://github.com/ryanbhayward/miowy/blob/master/node.cpp
#include "mcts.hpp"
#include "game.hpp"

#include "math.h"

void Node::expand(HexState& state) {}

int Node::bestChildIndex() { return 0; }

Player Node::randomPlayout(HexState state) {
   while(true) {
       int move = rand() % BOARD_SIZE;
       if (state.move(move)) {
           break;
       }
   } 
   return state.currentPlayer;
}

int Node::bestMove(HexState& state, int plays) { // random moves until someone wins
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (state.board[i] != CellState::Empty) {
            Node child = Node();
            child.lcn = i;
            children.push_back(child);
        }
    }
    int best_move, most_wins = -1;
    for (int i = 0; i < plays; i++) {
        for (int j = 0; j < children.size(); j++) {
            HexState new_state = HexState(state);
            new_state.move(children[j].lcn);
            children[j].record_game(state.currentPlayer == randomPlayout(new_state));
        }
    }
    for (int i = 0; i < children.size(); i++) {
        if (children[i].wins > most_wins) {
            most_wins = children[i].wins;
            best_move = children[i].lcn;
        }
    }
    return best_move;
}

int Node::uct_playout(HexState& state, int player, bool useMiai) { // does the monte carlo tree search
    if (isLeaf()) {
        if (games >= EXPAND_THRESHOLD) {
            expand(state);
        }
    }
    int winner;
    if (isLeaf()) {
        // PLAY GAME TO FIND WINNER
    } else {
        Node& c = children[bestChildIndex()];
        // todo
    }
    record_game(winner == player);
    return winner;
}
void Node::record_game(bool win) {
    wins += win;
    games++;
}

float Node::ucb_eval(Node& child) {
    if (games == 0) {
        return 1e10;  // "infinity"
    }
    return (1.0 - float(child.wins) / child.games) + UCB_EXPLORE * sqrt(sqrt(games) / child.games);
}