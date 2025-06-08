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
protected:
    int row;
    int col;
    bool is_white;
    int piece_value;
public:
    Piece(int row, int col, bool is_white);
    Piece(const Piece& other);
    Piece& operator=(const Piece& other);
    virtual bool isValidMove(int target_row, int target_col) = 0;
    virtual int getValue() const;
    int getRow() const;
    int getCol() const;
    bool getColor() const;
    void setRow(int row);
    void setCol(int col);
    virtual char getPieceName() const = 0;
    bool operator<(const Piece& other) const;
    virtual ~Piece() = default;
};


#endif //PIECE_H
