//
// Created by Erez on 07/04/2025.
//

#ifndef BISHOP_H
#define BISHOP_H


#include "Piece.h"

/**
 * Bishop class representing a bishop piece in chess.
 */
class Bishop :public Piece {

public:
    Bishop(int row, int col, bool is_white);
    bool isValidMove(int target_row, int target_col) override;
};



#endif //BISHOP_H
