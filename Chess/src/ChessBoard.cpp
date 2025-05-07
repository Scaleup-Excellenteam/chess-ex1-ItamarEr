
#include "ChessBoard.h"

#include <iostream>
#include <stdexcept>
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
#include "InvalidMoveException.h"
#include <climits>
#include <cmath>


/**
 * ctor for ChessBoard
 * @param boardString The string representation of the chessboard
 */
ChessBoard::ChessBoard(const string& boardString): isWhiteTurn(true) {

    chessBoard.resize(8, vector<Piece*>(8, nullptr));
    // Initialize the board with pieces
    int col_index = 0;
    int row_index = 0;
    for (const char c : boardString) {
        switch (c) {
            case '#':
                chessBoard[row_index][col_index] = nullptr;
                break;
            case 'P':
                chessBoard[row_index][col_index] = new Pawn(row_index, col_index, true);
                break;
            case 'B':
                chessBoard[row_index][col_index] = new Bishop(row_index, col_index, true);
                break;
            case 'R':
                chessBoard[row_index][col_index] = new Rook(row_index, col_index, true);
                break;
            case 'N':
                chessBoard[row_index][col_index] = new Knight(row_index, col_index, true);
                break;
            case 'Q':
                chessBoard[row_index][col_index] = new Queen(row_index, col_index, true);
                break;
            case 'K':
                chessBoard[row_index][col_index] = new King(row_index, col_index, true);
                break;
            case 'p':
                chessBoard[row_index][col_index] = new Pawn(row_index, col_index, false);
                break;
            case 'b':
                chessBoard[row_index][col_index] = new Bishop(row_index, col_index, false);
                break;
            case 'r':
                chessBoard[row_index][col_index] = new Rook(row_index, col_index, false);
                break;
            case 'n':
                chessBoard[row_index][col_index] = new Knight(row_index, col_index, false);
                break;
            case 'q':
                chessBoard[row_index][col_index] = new Queen(row_index, col_index, false);
                break;
            case 'k':
                chessBoard[row_index][col_index] = new King(row_index, col_index, false);
                break;
            default:
                throw invalid_argument("Invalid board string");
        }
        col_index++;
        if (col_index == 8) {
            col_index = 0;
            row_index++;
        }
    }

}

/**
 * Check if the move is valid for the piece at the source position
 * to the target position.
 * if the move is valid, it calls movePiece to move the piece.
 * @param source_row The row of the source position
 * @param source_col The column of the source position
 * @param target_row The row of the target position
 * @param target_col The column of the target position
 * @return A code indicating the result of the move.
 */
int ChessBoard::isValidMove(const int source_row, const int source_col, const int target_row, const int target_col) {
    if (chessBoard[source_row][source_col] == nullptr) {
        return 11; // There is no piece at the source
    }
    if (chessBoard[source_row][source_col]->getColor() != (isWhiteTurn)) {
        return 12; // The piece in the source is piece of your opponent
    }
    if (chessBoard[target_row][target_col] != nullptr &&
        chessBoard[source_row][source_col]->getColor() == chessBoard[target_row][target_col]->getColor()) {
        return 13; // There one of your pieces at the destination
    }
    if (!chessBoard[source_row][source_col]->isValidMove(target_row, target_col)) {
        return 21; // Illegal movement of that piece
    }

    if (!isPathClear(source_row, source_col, target_row, target_col)) {
        return 21; // Illegal movement of that piece
    }

    if (simulateMoveForCheck(source_row, source_col, target_row, target_col) == 31) {
        return 31; // This movement will cause checkmate
    }

    // Check if the pawn only captures diagonally
    if (dynamic_cast<Pawn*>(chessBoard[source_row][source_col]) != nullptr) {
        if (chessBoard[target_row][target_col] != nullptr) {
            if (source_col == target_col) {
                return 21; // Illegal movement of that piece
            }
        }
        else {
            if (abs(source_col - target_col) == 1) {
                return 21; // Illegal movement of that piece
            }
        }
    }
    bool checkFlag = false;
    string moveString;
    moveString += static_cast<char>('a' + source_row);
    moveString += static_cast<char>('1' + source_col);
    moveString += static_cast<char>('a' + target_row);
    moveString += static_cast<char>('1' + target_col);
    const auto sim = simulateMove(moveString);
    if (isKingInCheck(!isWhiteTurn)) {
        checkFlag = true;
    }
    undoMove(sim);
    if (checkFlag) {
        return 41; // The last movement was legal and caused check
    }
    return 42; // The last movement was legal, next turn

}

/**
 * Check if the king of the specified color is in check.
 * @param isWhite Color of the king to check (true for white, false for black)
 * @return true if the king is in check, false otherwise
 */
bool ChessBoard::isKingInCheck(const bool isWhite) const {
    int kingRow = -1, kingCol = -1;

    // Find the king's position
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (Piece* piece = chessBoard[row][col]; piece != nullptr
                && piece->getColor() == isWhite && dynamic_cast<King*>(piece) != nullptr) {
                kingRow = row;
                kingCol = col;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    // Check if any opponent piece can attack the king
    const bool opponent_color = !isWhite;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (Piece* piece = chessBoard[row][col]; piece != nullptr
                && piece->getColor() == opponent_color
                && piece->isValidMove(kingRow, kingCol)
                && isPathClear(row, col, kingRow, kingCol)) {
                return true;
            }
        }
    }
    return false;
}

/**
 * Check if the path between two positions is clear.
 * except for knights which can jump over pieces.
 * @param startRow The starting row
 * @param startCol The starting column
 * @param endRow The ending row
 * @param endCol The ending column
 * @return true if the path is clear, false otherwise
 */
bool ChessBoard::isPathClear(const int startRow, const int startCol, const int endRow, const int endCol) const {

    // Knights can jump over pieces, so we don't need to check the path for them
    if (dynamic_cast<Knight*>(chessBoard[startRow][startCol]) != nullptr) {return true;}

    // get the direction of the movement
    const int rowDirection = (endRow - startRow) == 0 ? 0 : (endRow - startRow) / abs(endRow - startRow);
    const int colDirection = (endCol - startCol) == 0 ? 0 : (endCol - startCol) / abs(endCol - startCol);

    int currentRow = startRow + rowDirection;
    int currentCol = startCol + colDirection;

    // Check the path until we reach the target position
    while (currentRow != endRow || currentCol != endCol) {
        if (chessBoard[currentRow][currentCol] != nullptr) {
            return false;
        }
        currentRow += rowDirection;
        currentCol += colDirection;
    }
    return true;
}

/**
 * convert a move string to an integer code and calls isValidMove.
 * @param moveString The move string in the format "a1b2"
 * @return the code of the move or throw an exception if the move is invalid.
 */
int ChessBoard::executeMove(const string& moveString) {
    if (moveString.length() != 4) {
        throw invalid_argument("Invalid move string length");
    }
    const int source_row = tolower(moveString[0]) - 'a';
    const int source_col = moveString[1] - '1';
    const int target_row = tolower(moveString[2]) - 'a';
    const int target_col = moveString[3] - '1';

    if (source_row < 0 || source_row > 7 || source_col < 0 || source_col > 7 ||
        target_row < 0 || target_row > 7 || target_col < 0 || target_col > 7) {
        throw invalid_argument("Invalid move string");
    }
    const int code = isValidMove(source_row, source_col, target_row, target_col);
    if (code == 41 || code == 42) {
        movePiece(source_row, source_col, target_row, target_col);
    }
    else {
        throw InvalidMoveException(code);
    }

    return code;
}

/**
 * switch the turn to the other player.
 */
void ChessBoard::switchTurn() {
    isWhiteTurn = !isWhiteTurn;
}

/**
 * make a move on the chessboard.
 * @param source_row The row of the source position
 * @param source_col The column of the source position
 * @param target_row The row of the target position
 * @param target_col The column of the target position
 */
void ChessBoard::movePiece(const int source_row, const int source_col, const int target_row, const int target_col) {
    if (chessBoard[target_row][target_col] != nullptr) {
        delete chessBoard[target_row][target_col];
    }
    chessBoard[source_row][source_col]->setRow(target_row);
    chessBoard[source_row][source_col]->setCol(target_col);
    chessBoard[target_row][target_col] = chessBoard[source_row][source_col];
    chessBoard[source_row][source_col] = nullptr;
    switchTurn();
}

/**
 * Simulate a move on the chessboard without actually moving the piece.
 * It checks if the move would cause checkmate.
 * @param source_row The row of the source position
 * @param source_col The column of the source position
 * @param target_row The row of the target position
 * @param target_col The column of the target position
 * @return 0 if the move is valid and does not cause checkmate,
 * 31 if the move would cause checkmate.
 */
int ChessBoard::simulateMoveForCheck(const int source_row, const int source_col, const int target_row, const int target_col) {
    // Simulate the move
    Piece* temp = chessBoard[target_row][target_col];
    chessBoard[target_row][target_col] = chessBoard[source_row][source_col];
    chessBoard[source_row][source_col] = nullptr;
    chessBoard[target_row][target_col]->setRow(target_row);
    chessBoard[target_row][target_col]->setCol(target_col);

    // Check if the move causes checkmate
    if (isKingInCheck(isWhiteTurn)) {
        // Revert the move
        chessBoard[source_row][source_col] = chessBoard[target_row][target_col];
        chessBoard[target_row][target_col] = temp;
        chessBoard[source_row][source_col]->setRow(source_row);
        chessBoard[source_row][source_col]->setCol(source_col);

        return 31; // This movement will cause checkmate
    }

    // Revert the move
    chessBoard[source_row][source_col] = chessBoard[target_row][target_col];
    chessBoard[target_row][target_col] = temp;
    chessBoard[source_row][source_col]->setRow(source_row);
    chessBoard[source_row][source_col]->setCol(source_col);

    return 0; // The move is valid and does not cause checkmate
}


/**
 * Get all valid moves for a given color.
 * @param forWhite true if getting moves for white, false if for black
 * @return A vector of strings representing the valid moves in the format "a1b2"
 */
vector<string> ChessBoard::getAllValidMoves(const bool forWhite)  {
    bool flag = false;
    if (forWhite != isWhiteTurn) {
        flag = true;
        switchTurn();
    }

    vector<string> validMoves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (chessBoard[row][col] != nullptr) {
                if (chessBoard[row][col]->getColor() == forWhite) {
                    for (int trg_row = 0; trg_row < 8; ++trg_row) {
                        for (int trg_col = 0; trg_col < 8; ++trg_col) {
                            const int code = isValidMove(row, col, trg_row, trg_col);
                            if (code == 41 || code == 42) {
                                string move;
                                move += static_cast<char>('a' + row);
                                move += static_cast<char>('1' + col);
                                move += static_cast<char>('a' + trg_row);
                                move += static_cast<char>('1' + trg_col);
                                validMoves.push_back(move);
                            }
                        }
                    }
                }
            }
        }
    }
    if (flag) {
        switchTurn();
    }
    return validMoves;
}

/**
 * Simulate a move for evaluation purposes.
 * @param moveString The move string in the format "a1b2"
 * @return A Move object representing the simulated move
 */
ChessBoard::Move ChessBoard::simulateMove(const string &moveString) {
    int fromRow = moveString[0] - 'a';
    int fromCol = moveString[1] - '1';
    int toRow = moveString[2] - 'a';
    int toCol = moveString[3] - '1';

    Piece* moved = chessBoard[fromRow][fromCol];
    Piece* captured = chessBoard[toRow][toCol];

    chessBoard[toRow][toCol] = moved;
    chessBoard[fromRow][fromCol] = nullptr;
    moved->setRow(toRow);
    moved->setCol(toCol);

    return {fromRow, fromCol, toRow, toCol, 0, captured };
}

/**
 * Undo a simulated move.
 * @param move The Move object representing the move to undo
 */
void ChessBoard::undoMove(const Move &move) {
    Piece* moved = chessBoard[move.target_row][move.target_col];
    chessBoard[move.source_row][move.source_col] = moved;
    moved->setRow(move.source_row);
    moved->setCol(move.source_col);

    chessBoard[move.target_row][move.target_col] = move.pieceCaptured;
}

/**
 * Evaluate a move based on various criteria.
 * @param move The move object
 * @return An integer score representing the evaluation of the move
 */
int ChessBoard::evaluateMove(const Move &move) {
    int score = 0;

    const int toRow = move.target_row;
    const int toCol = move.target_col;


    const Piece* piece = chessBoard[toRow][toCol];

    const bool isWhite = piece->getColor();

    const int checkmate = checkmateCheck(!isWhite);
    if (isWhite) {
        if (checkmate == 1) return INT_MIN; // White lost
        if (checkmate == 2) return INT_MAX; // Black lost
    } else {
        if (checkmate == 1) return INT_MAX; // White lost
        if (checkmate == 2) return INT_MIN; // Black lost
    }
    if (checkmate == 3) return 0; // Stalemate

    // Add the value of the piece being captured
    if (move.pieceCaptured != nullptr) {
        score += move.pieceCaptured->getValue();
    }

    // Add value if the piece moved is threaten a higher value piece
    const vector<Piece*> threats = getThreatsBy(isWhite ,toRow, toCol);
    for (Piece* target : threats) {
        if (target->getValue() > piece->getValue() && dynamic_cast<King*>(target) == nullptr) {
            score += (target->getValue() - piece->getValue()) / 10;
        }
    }

    // Deduct value if the piece is threatened by a lower value piece
    const vector<Piece*> threatenedBy = getThreatsOn(isWhite ,toRow, toCol);
    for (const Piece* attacker : threatenedBy) {
        if (attacker->getValue() < piece->getValue()) {
            score -= (piece->getValue() - attacker->getValue()) / 10;
        }
    }

    if (isKingInCheck(!isWhite)) {
        score += 10; // Add points for checking
    }

    // bonus for being in or moving to the central 4x4 area
    if (toRow >=2 && toRow <= 5 && toCol >= 2 && toCol <= 5) {
        score += 1; // Base bonus for being in the central area
    }
    // Additional bonus for being in or moving to the central 2x2 area
    if (toRow >= 3 && toRow <= 4 && toCol >= 3 && toCol <= 4) {
        score += 2;
    }

    return score;
}

/**
 * Minimax algorithm to evaluate the best move.
 * @param forWhite true if evaluating for white, false if for black
 * @param depth The depth of the search
 * @return The score of the best move
 */
int ChessBoard::minimax(const bool forWhite, const int depth) {
    if (depth == 0) return 0;

    vector<string> moves = getAllValidMoves(forWhite);
    if (moves.empty()) return 0;
    int bestScore = INT_MIN;

    for (const string& move : moves) {
        const int toRow = move[2] - 'a';
        const int toCol = move[3] - '1';
        // Score this move
        Move moveObj(move);
        if (chessBoard[toRow][toCol] != nullptr) {
            moveObj.setCapturedPiece(chessBoard[toRow][toCol]);
        }
        auto sim = simulateMove(move);

        int score = evaluateMove(moveObj);
        // recursive part
        const int reply = minimax(!forWhite, depth - 1);
        undoMove(sim);

        score -= reply;
        bestScore = max(bestScore, score);

    }

    return bestScore;
}

vector<ChessBoard::Move> ChessBoard::getAllMovesScores(const bool forWhite) {
    vector<Move> allMovesScores;
    vector<string> validMoves = getAllValidMoves(forWhite);

    for (const string& move : validMoves) {
        const int toRow = move[2] - 'a';
        const int toCol = move[3] - '1';

        Move moveObj(move);
        if (chessBoard[toRow][toCol] != nullptr) {
            moveObj.setCapturedPiece(chessBoard[toRow][toCol]);
        }
        auto sim = simulateMove(move);

        int score = evaluateMove(moveObj);

        const int replyScore = minimax(!forWhite, 2);

        undoMove(sim);
        // Subtract opponent's best reply
        score -= replyScore;
        moveObj.setValue(score);
        allMovesScores.push_back(moveObj);
    }

    return allMovesScores;
}


int ChessBoard::checkmateCheck(const bool forWhite) {
    const vector<string> allMoves = getAllValidMoves(forWhite);
    if (allMoves.empty()) {
        if (isKingInCheck(forWhite)) {
            return forWhite ? 1 : 2; // 1: White lost, 2: Black lost
        }
        return 3; // Stalemate
    }
    return 0; // No checkmate or stalemate
}

vector<Piece*> ChessBoard::getThreatsBy(const bool forWhite, const int row, const int col) const {
    vector<Piece*> piecesThreaten;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessBoard[i][j] != nullptr && chessBoard[i][j]->getColor() != forWhite) {
                if (chessBoard[row][col]->isValidMove(i, j) && isPathClear( row, col, i, j)) {
                    piecesThreaten.push_back(chessBoard[i][j]);
                }
            }
        }
    }
    return piecesThreaten;
}

vector<Piece *> ChessBoard::getThreatsOn(const bool forWhite, const int row, const int col) const {
    vector<Piece *> piecesThreat;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessBoard[i][j] != nullptr && chessBoard[i][j]->getColor() != forWhite) {
                if (chessBoard[i][j]->isValidMove(row, col) && isPathClear(i, j, row, col)) {
                    piecesThreat.push_back(chessBoard[i][j]);
                }
            }
        }
    }
    return piecesThreat;
}


/**
 * dtor for ChessBoard.
 * delete all pieces from the board.
 */
ChessBoard::~ChessBoard() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete chessBoard[i][j];
        }
    }
}

/**
 * Ctor for Move struct.
 * @param src_row row of the source piece
 * @param src_col column of the source piece
 * @param trg_row row of the target piece
 * @param trg_col column of the target piece
 * @param value the value of the move
 * @param capturedPiece the piece that was captured (if any)
 */
ChessBoard::Move::Move(const int src_row, const int src_col, const int trg_row,
    const int trg_col, const int value, Piece* capturedPiece):
        source_row(src_row), source_col(src_col), target_row(trg_row), target_col(trg_col), moveValue(value), pieceCaptured(capturedPiece) {}


/**
 * Ctor for Move struct.
 * @param move_str the move string in the format "a1b2"
 */
ChessBoard::Move::Move(const string &move_str) {
    source_row = tolower(move_str[0]) - 'a';
    source_col = move_str[1] - '1';
    target_row = tolower(move_str[2]) - 'a';
    target_col = move_str[3] - '1';
    if (source_row < 0 || source_row > 7 || source_col < 0 || source_col > 7 ||
        target_row < 0 || target_row > 7 || target_col < 0 || target_col > 7) {
        throw invalid_argument("Invalid move string");
    }
    moveValue = 0;
    pieceCaptured = nullptr;
}

/**
 * Convert the move to a string representation.
 * @return the move string in the format "a1b2"
 */
string ChessBoard::Move::getMoveString() const {
    string move_string;
    move_string += static_cast<char>('a' + this->source_row);
    move_string += static_cast<char>('1' + this->source_col);
    move_string += static_cast<char>('a' + this->target_row);
    move_string += static_cast<char>('1' + this->target_col);
    return move_string;

}

void ChessBoard::Move::setValue(const int value) {
    this->moveValue = value;
}

void ChessBoard::Move::setCapturedPiece(Piece* capturedPiece) {
    this->pieceCaptured = capturedPiece;
}

ostream& operator<<(ostream& os, const ChessBoard::Move& move) {
    os << move.getMoveString();
    return os;
}