
#include "ChessBoard.h"
#include <stdexcept>
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"


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

    if (simulateMove(source_row, source_col, target_row, target_col) == 31) {
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

    movePiece(source_row,source_col,target_row,target_col);

    if (isKingInCheck(isWhiteTurn)) {
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
int ChessBoard::stringMoveToInt(const string& moveString) {
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
    return isValidMove(source_row, source_col, target_row, target_col);
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
int ChessBoard::simulateMove(const int source_row, const int source_col, const int target_row, const int target_col) {
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
