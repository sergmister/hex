#include <cstdint>
#include <iostream>
#include <string>

#include "game.hpp"

uint16_t get_move_input(HexState state) {
    std::cout << (state.currentPlayer == CellState::Black ? "B" : "W")
              << " => ";
    std::string input;
    while (true) {
        std::cin >> input;
    }
}

int main(void) {
    HexBoard b = HexBoard();
    HexState h(b);
    h.print();
}