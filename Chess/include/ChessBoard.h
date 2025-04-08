//
// Created by Erez on 07/04/2025.
//

#ifndef CHESSBOARD_H
#define CHESSBOARD_H


#include <vector>
#include <string>
#include "Piece.h"

using namespace std;

/**
 * ChessBoard class represents a chessboard and manages the pieces on it.
 * It provides methods to check for valid moves, simulate moves, and manage turns.
 */

class ChessBoard {
    vector<vector<Piece*>> chessBoard;
    bool isWhiteTurn;

public:

    explicit ChessBoard(const string& boardString);
    bool isKingInCheck(bool isWhite) const;
    bool isPathClear(int startRow, int startCol, int endRow, int endCol) const;
    int isValidMove(int source_row, int source_col, int target_row, int target_col) ;
    int stringMoveToInt(const string& moveString);
    void switchTurn();
    void movePiece(int source_row, int source_col, int target_row, int target_col);
    int simulateMove(int source_row, int source_col, int target_row, int target_col);
    ~ChessBoard();

};


#endif //CHESSBOARD_H
