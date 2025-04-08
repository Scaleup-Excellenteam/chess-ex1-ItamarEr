//
// Created by Erez on 07/04/2025.
//

#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(int row, int col, bool is_white);
    bool isValidMove(int target_row, int target_col) override;
};



#endif //KNIGHT_H
