//
// Created by Erez on 07/04/2025.
//

#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

/**
 * Pawn class representing a pawn piece in chess.
 */
class Pawn : public Piece {
public:
    Pawn(int row, int col, bool is_white);
    bool isValidMove(int target_row, int target_col) override;

};


#endif //PAWN_H
