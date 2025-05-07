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

    /**
     *
     */
public:
    struct Move {
        int source_row;
        int source_col;
        int target_row;
        int target_col;
        int moveValue;
        Piece* pieceCaptured;
        Move(int src_row, int src_col, int trg_row, int trg_col, int value, Piece* capturedPiece=nullptr);

        explicit Move(const string &move_str);
        void setValue(int value);
        void setCapturedPiece(Piece* capturedPiece);
        string getMoveString() const;
        bool operator<(const Move& other) const {
            return moveValue < other.moveValue;
        }
        friend ostream& operator<<(ostream& os, const Move& move);
    };




    explicit ChessBoard(const string& boardString);
    bool isKingInCheck(bool isWhite) const;
    bool isPathClear(int startRow, int startCol, int endRow, int endCol) const;
    int isValidMove(int source_row, int source_col, int target_row, int target_col);
    int executeMove(const string& moveString);
    void switchTurn();
    void movePiece(int source_row, int source_col, int target_row, int target_col);
    int simulateMoveForCheck(int source_row, int source_col, int target_row, int target_col);

    vector<string> getAllValidMoves(bool forWhite);
    Move simulateMove(const string& moveString);
    void undoMove(const Move& move);
    int evaluateMove(const Move& move);
    int minimax(bool forWhite, int depth);
    vector<Piece*> getThreatsBy(bool forWhite, int row, int col) const;
    vector<Piece*> getThreatsOn(bool forWhite, int row, int col) const;


    int checkmateCheck(bool forWhite);
    vector<Move> getAllMovesScores(bool forWhite);

    ~ChessBoard();
};




#endif //CHESSBOARD_H
