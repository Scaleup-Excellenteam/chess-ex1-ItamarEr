//
// Created by Erez on 07/04/2025.
//

#include "Piece.h"

/**
 * ctor
 * @param row The row of the piece on the chessboard (0-7)
 * @param col The column of the piece on the chessboard (0-7)
 * @param is_white Boolean indicating if the piece is white (true) or black (false)
 */
Piece::Piece(const int row, const int col, const bool is_white): piece_value(0) {
    if (row < 0 || col < 0 || row > 8 || col > 8) {
        return;
    }
    this->row = row;
    this->col = col;
    this->is_white = is_white;

}

/**
 * copy ctor
 * @param other Piece to copy
 */
Piece::Piece(const Piece& other) {
    this->row = other.row;
    this->col = other.col;
    this->is_white = other.is_white;
    this->piece_value = other.piece_value;
}

/**
 * Assignment operator
 * @param other The piece to copy
 * @return Pointer to the current object with the copied values
 */
Piece& Piece::operator=(const Piece& other) {
    if (this != &other) {
        this->row = other.row;
        this->col = other.col;
        this->is_white = other.is_white;
        this->piece_value = other.piece_value;
    }
    return *this;
}

int Piece::getValue() const {
    return piece_value;
}

/**
 * row getter
 * @return The row of the piece on the chessboard (0-7)
 */
int Piece::getRow() const {
    return row;
}

/**
 * column getter
 * @return The column of the piece on the chessboard (0-7)
 */
int Piece::getCol() const {
    return col;
}

/**
 * color getter
 * @return true if the piece is white, false if black
 */
bool Piece::getColor() const {
    return is_white;
}

/**
 * row setter
 * @param row The new row of the piece on the chessboard (0-7)
 */
void Piece::setRow(const int row) {
    if (row < 0 || row > 7) {
        return;
    }
    this->row = row;
}

/**
 * column setter
 * @param col The new column of the piece on the chessboard (0-7)
 */
void Piece::setCol(const int col) {
    if (col < 0 || col > 7) {
        return;
    }
    this->col = col;
}

bool Piece::operator<(const Piece &other) const {
    return this->piece_value < other.piece_value;
}
