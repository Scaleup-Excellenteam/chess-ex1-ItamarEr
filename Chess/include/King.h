//
// Created by Erez on 07/04/2025.
//

#ifndef KING_H
#define KING_H


#include "Piece.h"

class King : public Piece {
public:
    King(int row, int col, bool is_white);
    bool isValidMove(int target_row, int target_col) override;

};


#endif //KING_H
