//
// Created by Erez on 07/04/2025.
//

#include "Pawn.h"

/**
 * ctor
 * @param row The row of the piece on the chessboard (0-7)
 * @param col The column of the piece on the chessboard (0-7)
 * @param is_white Boolean indicating if the piece is white (true) or black (false)
 */
Pawn::Pawn(const int row, const int col, const bool is_white):
    Piece(row, col, is_white) {

}

/**
 * Checks if the move is valid for a Pawn.
 * It's only check if the move is forward one square or two squares from its starting position.
 * Also checks if the pawn can capture diagonally.
 * @param target_row The target row to move to
 * @param target_col The target column to move to
 * @return true if the move is valid, false otherwise
 */
bool Pawn::isValidMove(const int target_row, const int target_col) {
    const int source_row = getRow();
    const int source_col = getCol();

    if (getColor() ) {
        // Check if the white pawn is moving forward / capture in diagonal
        if (target_row == source_row + 1 && abs(source_col - target_col) <= 1) {
            return true;
        }
        // Check if the white pawn is moving two squares forward from its starting position
        if (source_row == 1 && target_row == source_row + 2 && source_col == target_col) {
            return true;
        }
    } else {
        // Check if the black pawn is moving forward / capture in diagonal
        if (target_row == source_row - 1 && abs(source_col - target_col) <= 1) {
            return true;
        }
        // Check if the black pawn is moving two squares forward from its starting position
        if (source_row == 6 && target_row == source_row - 2 && source_col == target_col) {
            return true;
        }
    }
    return false;
}

