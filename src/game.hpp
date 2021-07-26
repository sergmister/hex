#pragma once
#include <cstdint>

const uint16_t BOARD_WIDTH = 2;
const uint16_t BOARD_HEIGHT = 2;
const uint16_t BOARD_SIZE = BOARD_WIDTH * BOARD_HEIGHT;

inline uint16_t IX(uint16_t x, uint16_t y) { return y * BOARD_WIDTH + x; }

enum struct Player : int8_t {
    White,
    Black,
};

enum struct CellState : int8_t {
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

struct HexState {
    bool gameOver = false;
    HexState();

    HexState(HexState &hexState);

    Player currentPlayer;

    CellState board[BOARD_SIZE];

    void reset();

    void copy_from(HexState &hexState);

    void print();
};

struct HexBoard {
    HexBoard();

    Neighbor neighbor_list[BOARD_SIZE];

    bool move(HexState &state, uint16_t pos);

    void dfs(HexState &state, uint16_t pos, CellState eq_cell_state, CellState move_cell_state);
};
