#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include "arena.hpp"
#include "game.hpp"
#include "mcts.hpp"

extern const uint16_t BOARD_WIDTH;
extern const uint16_t BOARD_HEIGHT;
extern const uint16_t BOARD_SIZE;

uint16_t get_move_input(HexState state) {
    std::string input;
    while (true) {
        std::cout << (state.currentPlayer == Player::Black ? "B" : "W") << " => ";
        getline(std::cin, input);
        if (input.length() < 2) {
            std::cout << "invalid" << std::endl;
            continue;
        }
        char letter = input.at(0);
        uint16_t number;
        try {
            number = std::stoi(input.substr(1));
        } catch (...) {
            std::cout << "invalid" << std::endl;
            continue;
        }
        uint16_t x, y;
        if (letter >= 'a' && letter < ('a' + BOARD_WIDTH)) {
            x = letter - 'a';
        } else {
            std::cout << "invalid" << std::endl;
            continue;
        }
        if (number >= 1 && number <= BOARD_HEIGHT) {
            y = number - 1;
        } else {
            std::cout << "invalid" << std::endl;
            continue;
        }
        uint16_t pos = IX(x, y);
        if (state.board[pos] == CellState::Empty) {
            return pos;
        } else {
            std::cout << "invalid" << std::endl;
            continue;
        }
    }
}

int main() {
    HexBoard board = HexBoard();
    HexState state = HexState();
    MCTS m(&state);

    std::cout << "Black (x): top-bottom\n White (o): left-right\n" << std::endl;

    while (true) {
        state.print();
        uint16_t move;
        if (state.currentPlayer == Player::Black) {
            move = m.best_move();
        } else {
            move = get_move_input(state);
        }
        m.move(move);
        if (board.move(state, move)) {
            break;
        }
    }
    state.print();
    std::cout << (state.currentPlayer == Player::Black ? "Black" : "White") << " wins!" << std::endl;
}
