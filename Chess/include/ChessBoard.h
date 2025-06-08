//
// Created by Erez on 07/04/2025.
//

#ifndef CHESSBOARD_H
#define CHESSBOARD_H


#include <condition_variable>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include "Piece.h"
#include "PriorityQueue.h"

using namespace std;

/**
 * ChessBoard class represents a chessboard and manages the pieces on it.
 * It provides methods to check for valid moves, simulate moves, and manage turns.
 */

class ChessBoard {


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

    ChessBoard(const string& boardString,int depth);

    vector<vector<Piece *>> cloneBoard() const;

    bool isKingInCheck(bool isWhite, const vector<vector<Piece*>>& localBoard) const;
    bool isPathClear(int startRow, int startCol, int endRow, int endCol
        , const vector<vector<Piece*>>& localBoard) const;

    int isValidMove(int source_row, int source_col, int target_row, int target_col
        , vector<vector<Piece*>>& localBoard,bool forWhite) const;

    int executeMove(const string& moveString);
    void switchTurn();
    void movePiece(int source_row, int source_col, int target_row, int target_col);
    int simulateMoveForCheck(int source_row, int source_col, int target_row, int target_col,
       vector<vector<Piece*>>& localBoard ) const;

    vector<string> getValidMovesForPiece(const Piece* piece, vector<vector<Piece*>>& localBoard) const;
    vector<string> getAllValidMoves(bool forWhite,  vector<vector<Piece*>>& localBoard) const;
    Move simulateMove(const string& moveString,vector<vector<Piece*>>& localBoard) const;
    void undoMove(const Move& move,vector<vector<Piece*>>& localBoard) const;
    int evaluateMove(const Move& move, vector<vector<Piece*>>& localBoard) const;
    int minimax(bool forWhite, int depth,int alpha, int beta, vector<vector<Piece*>>& localBoard);
    vector<Piece*> getThreatsBy(bool forWhite, int row, int col
        ,const vector<vector<Piece*>>& localBoard) const;\

    vector<Piece*> getThreatsOn(bool forWhite, int row, int col
        , const vector<vector<Piece*>>& localBoard) const;
    
    bool getIsWhiteTurn() const;
    string getBestMove();
    int checkmateCheck(bool forWhite, vector<vector<Piece*>>& localBoard) const;
    void printBestMoves() const;
    void updatePriorityQueue();
    void initThreadPool(int numThreads);
    void waitForThreads();
    ~ChessBoard();

private:
    vector<vector<Piece*>> chessBoard;
    bool isWhiteTurn;
    int depth;
    PriorityQueue<Move> pq;
    mutex threadMutex;
    int threadsCompleted;
    condition_variable threadCondition;
    int numThreads;
};




#endif //CHESSBOARD_H
