//
// Created by Erez on 07/04/2025.
//

#ifndef KING_H
#define KING_H


#include "Piece.h"

/**
 * King class representing a king piece in chess.
 */
class King : public Piece {
public:
    King(int row, int col, bool is_white);
    bool isValidMove(int target_row, int target_col) override;
    char getPieceName() const override;
};


#endif //KING_H
