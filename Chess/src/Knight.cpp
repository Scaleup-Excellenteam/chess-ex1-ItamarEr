//
// Created by Erez on 07/04/2025.
//

#include "Knight.h"

/**
 * ctor
 * @param row The row of the piece on the chessboard (0-7)
 * @param col The column of the piece on the chessboard (0-7)
 * @param is_white Boolean indicating if the piece is white (true) or black (false)
 */
Knight::Knight(const int row, const int col, const bool is_white):
Piece(row, col, is_white) {
    piece_value = 30;
}

/**
 * Checks if the move is valid for a Knight.
 * It's only check if the move is 'L' shape
 * (2 squares in one direction and 1 square in the other).
 * @param target_row The target row to move to
 * @param target_col The target column to move to
 * @return true if the move is valid, false otherwise
 */
bool Knight::isValidMove(const int target_row, const int target_col) {
    const int source_row = getRow();
    const int source_col = getCol();

    // Check if the move is in an "L" shape
    if ((abs(source_row - target_row) == 2 && abs(source_col - target_col) == 1) ||
        (abs(source_row - target_row) == 1 && abs(source_col - target_col) == 2)) {
        return true;
        }
    return false;
}

char Knight::getPieceName() const {
    return is_white ? 'N' : 'n';
}
