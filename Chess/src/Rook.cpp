//
// Created by Erez on 07/04/2025.
//

#include "Rook.h"

/**
 * ctor
 * @param row The row of the piece on the chessboard (0-7)
 * @param col The column of the piece on the chessboard (0-7)
 * @param is_white Boolean indicating if the piece is white (true) or black (false)
 */
Rook::Rook(const int row, const int col, const bool is_white)
: Piece(row, col, is_white) {
    piece_value = 50;
}

/**
 * Checks if the move is valid for a Rool.
 * It's only check if the move is horizontal or vertical.
 * @param target_row The target row to move to
 * @param target_col The target column to move to
 * @return true if the move is valid, false otherwise
 */
bool Rook::isValidMove(const int target_row, const int target_col) {
    const int source_row = getRow();
    const int source_col = getCol();

    // Check if the move is horizontal
    if (source_row == target_row && source_col != target_col) {
        return true;
    }
    // Check if the move is vertical
    if (source_row != target_row && source_col == target_col) {
        return true;
    }
    return false;
}

char Rook::getPieceName() const {
    return is_white ? 'R' : 'r';
}
