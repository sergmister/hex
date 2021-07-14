#include "arena.hpp"

#include <iostream>
#include <vector>

#include "mcts.hpp"

void Arena::play_games(int amount) {
    int win1 = 0, win2 = 0;
    for (int i = 0; i < amount; i++) {
        play_game(i % 2 == 0);
    }
    print();
}

void Arena::play_game(bool playBlack) {
    HexBoard board = HexBoard();
    HexState state = HexState();
    MCTS mcts;
    int move;
    while (true) {
        if ((state.currentPlayer == Player::Black) == playBlack) {
            move = mcts.best_move(state);
        } else {
            std::vector<int> moves;
            for (int i = 0; i < BOARD_SIZE; i++) {
                if (state.board[i] == CellState::Empty) {
                    moves.push_back(i);
                }
            }
            srand(time(NULL));
            move = moves[rand() % moves.size()];
        }
        if (board.move(state, move)) {
            break;
        }
    }
    if ((state.currentPlayer == Player::Black) == playBlack) {
        win1++;
    } else {
        win2++;
    }
}

void Arena::print() {
    std::cout << "Player 1: " << win1 << "/" << win1 + win2 << '\n';
    std::cout << "Player 2: " << win2 << "/" << win1 + win2 << '\n';
}