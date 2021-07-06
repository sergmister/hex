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
    HexState s = HexState(b);
    s.currentPlayer = CellState::White;
    std::cout << s.move(0) << std::endl;
    std::cout << s.move(1) << std::endl;
    std::cout << s.move(2) << std::endl;
    std::cout << s.move(3) << std::endl;
    s.print();
}