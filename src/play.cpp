#include <iostream>

#include "game.hpp"

int main(void) {
    HexBoard b;
    HexState h(b);
    h.print();
}