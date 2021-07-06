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

int main() {
    HexBoard b = HexBoard();
    HexState s = HexState(b);
    s.currentPlayer = CellState::Black;
    std::cout << s.move(0) << std::endl;
    std::cout << s.move(1) << std::endl;
    std::cout << s.move(2) << std::endl;
    std::cout << s.move(3) << std::endl;
    std::cout << s.move(4) << std::endl;
    std::cout << s.move(8) << std::endl;
    std::cout << s.move(12) << std::endl;
    std::cout << s.move(16) << std::endl;
    // std::cout << s.move(20) << std::endl;
    s.print();
}