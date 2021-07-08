#include "game.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

extern const uint16_t BOARD_WIDTH;
extern const uint16_t BOARD_HEIGHT;
extern const uint16_t BOARD_SIZE;

HexBoard::HexBoard() {
    for (uint16_t x = 0; x < BOARD_WIDTH; x++) {
        for (uint16_t y = 0; y < BOARD_HEIGHT; y++) {
            Neighbor& cell = neighbor_list[IX(x, y)];
            cell.size = 0;
            if (x > 0) {
                cell.neighbors[cell.size++] = IX(x - 1, y);
            }
            if (y > 0) {
                cell.neighbors[cell.size++] = IX(x, y - 1);
            }
            if (x < (BOARD_WIDTH - 1)) {
                cell.neighbors[cell.size++] = IX(x + 1, y);
            }
            if (y < (BOARD_HEIGHT - 1)) {
                cell.neighbors[cell.size++] = IX(x, y + 1);
            }
            if (x > 0 && y < (BOARD_HEIGHT - 1)) {
                cell.neighbors[cell.size++] = IX(x - 1, y + 1);
            }
            if (y > 0 && x < (BOARD_WIDTH - 1)) {
                cell.neighbors[cell.size++] = IX(x + 1, y - 1);
            }
        }
    }
}

HexState::HexState() : currentPlayer(Player::Black) { std::fill_n(board, BOARD_SIZE, CellState::Empty); }

HexState::HexState(HexState& hexState) : currentPlayer(hexState.currentPlayer) {
    std::copy(std::begin(hexState.board), std::end(hexState.board), std::begin(this->board));
}

void HexState::reset() {
    currentPlayer = Player::Black;
    std::fill_n(board, BOARD_SIZE, CellState::Empty);
}

void HexState::copy_from(HexState& hexState) {
    currentPlayer = hexState.currentPlayer;
    std::copy(std::begin(hexState.board), std::end(hexState.board), std::begin(this->board));
}

// closures are complicated and sometimes slow
// passing eq_cell_state and move_cell_state by struct is 20% slower than by argument
// could this be something with the cache? (cpus are weird)

void HexBoard::dfs(HexState& state, uint16_t pos, CellState eq_cell_state, CellState move_cell_state) {
    Neighbor neighbor_cell = neighbor_list[pos];
    for (uint8_t i = 0; i < neighbor_cell.size; i++) {
        if (state.board[neighbor_cell.neighbors[i]] == eq_cell_state) {
            state.board[neighbor_cell.neighbors[i]] = move_cell_state;
            dfs(state, neighbor_cell.neighbors[i], eq_cell_state, move_cell_state);
        }
    }
}

bool HexBoard::move(HexState& state, uint16_t pos) {
    // if (board[pos] != CellState::Empty) return false;
    switch (state.currentPlayer) {
        case Player::Black: {
            bool north_connected = false;
            bool south_connected = false;
            if (pos < BOARD_WIDTH) {  // First row
                north_connected = true;
            } else if (pos >= (BOARD_SIZE - BOARD_WIDTH)) {  // Last row
                south_connected = true;
            }
            Neighbor neighbor_cell = neighbor_list[pos];
            for (uint8_t i = 0; i < neighbor_cell.size; i++) {
                if (state.board[neighbor_cell.neighbors[i]] == CellState::BlackNorth) {
                    north_connected = true;
                } else if (state.board[neighbor_cell.neighbors[i]] == CellState::BlackSouth) {
                    south_connected = true;
                }
            }
            if (north_connected && south_connected) {
                state.board[pos] = CellState::BlackWin;
                state.gameOver = true;
                return true;
            } else if (north_connected) {
                state.board[pos] = CellState::BlackNorth;
                dfs(state, pos, CellState::Black, CellState::BlackNorth);
            } else if (south_connected) {
                state.board[pos] = CellState::BlackSouth;
                dfs(state, pos, CellState::Black, CellState::BlackNorth);
            } else {
                state.board[pos] = CellState::Black;
            }
            break;
        }
        case Player::White: {
            bool west_connected = false;
            bool east_connected = false;
            if (pos % BOARD_WIDTH == 0) {  // First column
                west_connected = true;
            } else if (pos % BOARD_WIDTH == (BOARD_WIDTH - 1)) {  // Last column
                east_connected = true;
            }
            Neighbor neighbor_cell = neighbor_list[pos];
            for (uint8_t i = 0; i < neighbor_cell.size; i++) {
                if (state.board[neighbor_cell.neighbors[i]] == CellState::WhiteWest) {
                    west_connected = true;
                } else if (state.board[neighbor_cell.neighbors[i]] == CellState::WhiteEast) {
                    east_connected = true;
                }
            }
            if (west_connected && east_connected) {
                state.board[pos] = CellState::WhiteWin;
                state.gameOver = true;
                return true;
            } else if (west_connected) {
                state.board[pos] = CellState::WhiteWest;
                dfs(state, pos, CellState::Black, CellState::BlackNorth);
            } else if (east_connected) {
                state.board[pos] = CellState::WhiteEast;
                dfs(state, pos, CellState::Black, CellState::BlackNorth);
            } else {
                state.board[pos] = CellState::White;
            }
            break;
        }
    }

    state.currentPlayer = state.currentPlayer == Player::Black ? Player::White : Player::Black;
    return false;
}

std::string StateToString(CellState state) {
    // switch (state) {
    //     case CellState::Empty:
    //         return ".";
    //     case CellState::White:
    //         return "o";
    //     case CellState::WhiteWin:
    //         return "O";
    //     case CellState::WhiteWest:
    //         return "p";
    //     case CellState::WhiteEast:
    //         return "q";
    //     case CellState::Black:
    //         return "x";
    //     case CellState::BlackWin:
    //         return "X";
    //     case CellState::BlackNorth:
    //         return "y";
    //     case CellState::BlackSouth:
    //         return "z";
    //     default:
    //         return "This will never return.";
    // }
    switch (state) {
        case CellState::Empty:
            return ".";
        case CellState::White:
            return "o";
        case CellState::WhiteWin:
            return "O";
        case CellState::WhiteWest:
            return "o";
        case CellState::WhiteEast:
            return "o";
        case CellState::Black:
            return "x";
        case CellState::BlackWin:
            return "X";
        case CellState::BlackNorth:
            return "x";
        case CellState::BlackSouth:
            return "x";
        default:
            return "This will never return.";
    }
}

void HexState::print() {
    std::string str;
    for (int i = 0; i < BOARD_WIDTH; i++) {
        str += " ";
        str += (char)('a' + i);
    }
    str += "\n";
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int i = 0; i < y; i++) {
            str += " ";
        }
        str += std::to_string(y + 1);
        for (int x = 0; x < BOARD_WIDTH; x++) {
            str += " ";
            str += StateToString(board[IX(x, y)]);
        }
        str += "\n";
    }
    std::cout << str;
}