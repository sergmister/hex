#pragma once
#include <cstdint>

const uint16_t BOARD_WIDTH = 6;
const uint16_t BOARD_HEIGHT = 6;
const uint16_t BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;

inline uint16_t IX(uint16_t x, uint16_t y) { return y * BOARD_WIDTH + x; }

enum struct CellState {
    Empty = 0,
    WhiteWest = -3,
    WhiteEast = -2,
    WhiteWin = -4,
    White = -1,
    BlackNorth = 3,
    BlackSouth = 2,
    BlackWin = 4,
    Black = 1,
};

struct Neighbor {
    uint8_t size;
    uint16_t neighbors[6];
};

struct HexBoard {
    HexBoard();

    Neighbor neighbor_list[BOARD_SIZE];
};

// struct Move {};

struct HexState {
    HexState(HexBoard &hexBoard);

    HexBoard hexBoard;

    CellState currentPlayer;

    CellState board[BOARD_SIZE];

    bool move(uint16_t pos);

    void bfs(uint16_t pos, CellState move_cell_state);

    void print();
};
