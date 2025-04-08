//
// Created by Erez on 07/04/2025.
//

#ifndef PIECE_H
#define PIECE_H

#include <stdlib.h>
using namespace std;


/**
 * Abstract class representing a chess piece.
 */
class Piece {

    int row;
    int col;
    bool is_white;
public:
    Piece(int row, int col, bool is_white);
    Piece(const Piece& other);
    Piece& operator=(const Piece& other);
    virtual bool isValidMove(int target_row, int target_col) = 0;
    int getRow() const;
    int getCol() const;
    bool getColor() const;
    void setRow(int row);
    void setCol(int col);
    virtual ~Piece() = default;
};


#endif //PIECE_H
