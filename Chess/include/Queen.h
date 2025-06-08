//
// Created by Erez on 07/04/2025.
//

#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

/**
 * Queen class representing a queen piece in chess.
 */
class Queen : public Piece {
public:
    Queen(int row, int col, bool is_white);
    bool isValidMove(int target_row, int target_col) override;
    char getPieceName() const override;
};


#endif //QUEEN_H
