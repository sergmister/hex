#include "game.hpp"

#include <algorithm>
#include <cstdint>
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

HexState::HexState(HexBoard& hexBoard)
    : hexBoard(hexBoard), currentPlayer(CellState::Black) {
    std::fill_n(board, BOARD_SIZE, CellState::Empty);
}

void HexState::bfs(uint16_t pos, CellState move_cell_state) {
    // Move is connected to an edge but not winning.
    // Update edge-connected groups with a flood-fill, to maintain that all edge
    // connected nodes are known about.
    // We don't do flood fill if a player has won, so it's impossible for a cell
    // connected to an edge to be changed by the flood-fill.
    // We assume that move can safely be cast to int
    std::vector<uint16_t> flood_stack = {pos};
    uint16_t latest_cell;
    while (!flood_stack.empty()) {
        latest_cell = flood_stack.back();
        flood_stack.pop_back();
        Neighbor neighbor_cell = hexBoard.neighbor_list[latest_cell];
        for (uint8_t i = 0; i < neighbor_cell.size; i++) {
            if (board[neighbor_cell.neighbors[i]] == currentPlayer) {
                // We make the change before putting the cell on the queue to
                // avoid putting the same cell on the queue multiple times
                board[neighbor_cell.neighbors[i]] = move_cell_state;
                flood_stack.push_back(neighbor_cell.neighbors[i]);
            }
        }
    }
}

bool HexState::move(uint16_t pos) {
    // if (board[pos] != CellState::Empty) return false;

    // board[pos] = currentPlayer;

    switch (currentPlayer) {
        case CellState::Black: {
            bool north_connected = false;
            bool south_connected = false;
            if (pos < BOARD_WIDTH) {  // First row
                north_connected = true;
            } else if (pos >= (BOARD_SIZE - BOARD_WIDTH)) {  // Last row
                south_connected = true;
            }
            for (uint8_t i = 0; i < hexBoard.neighbor_list[pos].size; i++) {
                if (board[hexBoard.neighbor_list[pos].neighbors[i]] ==
                    CellState::BlackNorth) {
                    north_connected = true;
                } else if (board[hexBoard.neighbor_list[pos].neighbors[i]] ==
                           CellState::BlackSouth) {
                    south_connected = true;
                }
            }
            if (north_connected && south_connected) {
                board[pos] = CellState::BlackWin;
                return true;
            } else if (north_connected) {
                board[pos] = CellState::BlackNorth;
                bfs(pos, CellState::BlackNorth);
            } else if (south_connected) {
                board[pos] = CellState::BlackSouth;
                bfs(pos, CellState::BlackSouth);
            } else {
                board[pos] = CellState::Black;
            }
            break;
        }
        case CellState::White: {
            bool west_connected = false;
            bool east_connected = false;
            if (pos % BOARD_WIDTH == 0) {  // First column
                west_connected = true;
            } else if (pos % BOARD_WIDTH == BOARD_WIDTH - 1) {  // Last column
                east_connected = true;
            }
            for (uint8_t i = 0; i < hexBoard.neighbor_list[pos].size; i++) {
                if (board[hexBoard.neighbor_list[pos].neighbors[i]] ==
                    CellState::WhiteWest) {
                    west_connected = true;
                } else if (board[hexBoard.neighbor_list[pos].neighbors[i]] ==
                           CellState::WhiteEast) {
                    east_connected = true;
                }
            }
            if (west_connected && east_connected) {
                board[pos] = CellState::WhiteWin;
                return true;
            } else if (west_connected) {
                board[pos] = CellState::WhiteWest;
                bfs(pos, CellState::WhiteWest);
            } else if (east_connected) {
                board[pos] = CellState::WhiteEast;
                bfs(pos, CellState::WhiteEast);
            } else {
                board[pos] = CellState::White;
            }
            break;
        }
    }

    currentPlayer =
        currentPlayer == CellState::Black ? CellState::White : CellState::Black;
    return false;
}

std::string StateToString(CellState state) {
    switch (state) {
        case CellState::Empty:
            return ".";
        case CellState::White:
            return "o";
        case CellState::WhiteWin:
            return "O";
        case CellState::WhiteWest:
            return "p";
        case CellState::WhiteEast:
            return "q";
        case CellState::Black:
            return "x";
        case CellState::BlackWin:
            return "X";
        case CellState::BlackNorth:
            return "y";
        case CellState::BlackSouth:
            return "z";
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