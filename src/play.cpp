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

main(void) {
    HexBoard b = HexBoard();
    std::cout << b.neighbor_list << std::endl;
}