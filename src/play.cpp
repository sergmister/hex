#include <cstdint>
#include <iostream>
#include <string>

#include "game.hpp"

extern const uint16_t BOARD_WIDTH;
extern const uint16_t BOARD_HEIGHT;
extern const uint16_t BOARD_SIZE;

uint16_t get_move_input(HexState state) {
    std::string input;
    while (true) {
        std::cout << (state.currentPlayer == CellState::Black ? "B" : "W")
                  << " => ";
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
    HexBoard b = HexBoard();
    HexState s = HexState(b);

    while (true) {
        s.print();
        uint16_t move = get_move_input(s);
        if (s.move(move)) {
            break;
        }
    }
    s.print();
    std::cout << (s.currentPlayer == CellState::Black ? "Black" : "White")
              << " wins!" << std::endl;
}