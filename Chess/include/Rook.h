//
// Created by Erez on 07/04/2025.
//

#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

/**
 * Rook class representing a rook piece in chess.
 */
class Rook: public Piece {
public:
    Rook(int row, int col, bool is_white);
    bool isValidMove(int target_row, int target_col) override;
    char getPieceName() const override;
};



#endif //ROOK_H
