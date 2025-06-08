
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
 * @param depth The depth of the search for the AI (default is 4)
 */
ChessBoard::ChessBoard(const string& boardString, const int depth)
: isWhiteTurn(true),depth(depth), threadsCompleted(0)
,numThreads(1), fiftyMoveRuleCounter(0), isGameDrawn(false), isInsufficientMaterial(false) {

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
 * Function to clone the chessboard including the pieces.
 * @return The cloned chessboard as a vector of vectors of Piece pointers.
 */
vector<vector<Piece*>> ChessBoard::cloneBoard() const {
    vector newBoard(8, vector<Piece*>(8, nullptr));
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessBoard[i][j]) {
                Piece* original = chessBoard[i][j];

                if (dynamic_cast<Pawn*>(original)) {
                    newBoard[i][j] = new Pawn(i, j, original->getColor());
                } else if (dynamic_cast<Rook*>(original)) {
                    newBoard[i][j] = new Rook(i, j, original->getColor());
                } else if (dynamic_cast<Knight*>(original)) {
                    newBoard[i][j] = new Knight(i, j, original->getColor());
                } else if (dynamic_cast<Bishop*>(original)) {
                    newBoard[i][j] = new Bishop(i, j, original->getColor());
                } else if (dynamic_cast<Queen*>(original)) {
                    newBoard[i][j] = new Queen(i, j, original->getColor());
                } else if (dynamic_cast<King*>(original)) {
                    newBoard[i][j] = new King(i, j, original->getColor());
                }

                // copy all properties using operator=
                *newBoard[i][j] = *original;
            }
        }
    }
    return newBoard;
}


/**
 * Check if the move is valid for the piece at the source position
 * to the target position.
 * if the move is valid, it calls movePiece to move the piece.
 * @param source_row The row of the source position
 * @param source_col The column of the source position
 * @param target_row The row of the target position
 * @param target_col The column of the target position
 * @param localBoard A reference to the local board state to check the move against
 * @param forWhite true if the moving piece is white, false if black
 * @return A code indicating the result of the move.
 */
int ChessBoard::isValidMove(const int source_row, const int source_col, const int target_row, const int target_col,
     vector<vector<Piece*>>& localBoard, const bool forWhite) const {
    if (localBoard[source_row][source_col] == nullptr) {
        return 11; // There is no piece at the source
    }
    if (localBoard[source_row][source_col]->getColor() != (forWhite)) {
        return 12; // The piece in the source is piece of your opponent
    }
    if (localBoard[target_row][target_col] != nullptr &&
        localBoard[source_row][source_col]->getColor() == localBoard[target_row][target_col]->getColor()) {
        return 13; // There one of your pieces at the destination
    }
    if (!localBoard[source_row][source_col]->isValidMove(target_row, target_col)) {
        return 21; // Illegal movement of that piece
    }

    if (!isPathClear(source_row, source_col, target_row, target_col, localBoard)) {
        return 21; // Illegal movement of that piece
    }

    if (simulateMoveForCheck(source_row, source_col, target_row, target_col,localBoard) == 31) {
        return 31; // This movement will cause checkmate
    }

    // Check if the pawn only captures diagonally
    if (dynamic_cast<Pawn*>(localBoard[source_row][source_col]) != nullptr) {
        if (localBoard[target_row][target_col] != nullptr) {
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
    const auto sim = simulateMove(moveString,localBoard);
    if (isKingInCheck(!forWhite, localBoard)) {
        checkFlag = true;
    }
    undoMove(sim,localBoard);
    if (checkFlag) {
        return 41; // The last movement was legal and caused check
    }
    return 42; // The last movement was legal, next turn

}

/**
 * Check if the king of the specified color is in check.
 * @param isWhite Color of the king to check (true for white, false for black)
 * @param localBoard A reference to the local board state to check for check
 * @return true if the king is in check, false otherwise
 */
bool ChessBoard::isKingInCheck(const bool isWhite, const vector<vector<Piece*>>& localBoard) const {
    int kingRow = -1, kingCol = -1;

    // Find the king's position
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (Piece* piece = localBoard[row][col]; piece != nullptr
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
            if (Piece* piece = localBoard[row][col]; piece != nullptr
                && piece->getColor() == opponent_color
                && piece->isValidMove(kingRow, kingCol)
                && isPathClear(row, col, kingRow, kingCol, localBoard)) {
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
 * @param localBoard A reference to the local board state to check the path against
 * @return true if the path is clear, false otherwise
 */
bool ChessBoard::isPathClear(const int startRow, const int startCol, const int endRow, const int endCol,
    const vector<vector<Piece*>>& localBoard) const {

    // Knights can jump over pieces, so we don't need to check the path for them
    if (dynamic_cast<Knight*>(localBoard[startRow][startCol]) != nullptr) {return true;}

    // get the direction of the movement
    const int rowDirection = (endRow - startRow) == 0 ? 0 : (endRow - startRow) / abs(endRow - startRow);
    const int colDirection = (endCol - startCol) == 0 ? 0 : (endCol - startCol) / abs(endCol - startCol);

    int currentRow = startRow + rowDirection;
    int currentCol = startCol + colDirection;

    // Check the path until we reach the target position
    while (currentRow != endRow || currentCol != endCol) {
        if (localBoard[currentRow][currentCol] != nullptr) {
            return false;
        }
        currentRow += rowDirection;
        currentCol += colDirection;
    }
    return true;
}

/**
 * Function to execute a move on the chessboard.
 * It checks if the move is valid and then moves the piece.
 * @param moveString The move string in the format "a1b2"
 * @return the code of the move or throw an exception if the move is invalid.
 * @throws invalid_argument if the move string is invalid or has an incorrect length.
 * @throws InvalidMoveException if the move is invalid according to the game rules.
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
    const int code = isValidMove(source_row, source_col, target_row, target_col, chessBoard, isWhiteTurn);
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
        fiftyMoveRuleCounter = 0; // Reset the fifty-move rule counter if a piece is captured
    }
    if (dynamic_cast<Pawn*>(chessBoard[source_row][source_col]) != nullptr) {
        fiftyMoveRuleCounter = 0; // Reset the fifty-move rule counter if a pawn is moved
    }
    chessBoard[source_row][source_col]->setRow(target_row);
    chessBoard[source_row][source_col]->setCol(target_col);
    chessBoard[target_row][target_col] = chessBoard[source_row][source_col];
    chessBoard[source_row][source_col] = nullptr;
    switchTurn();
    const string boardString = getBoardString();
    if (boardRepetition.contains(boardString)) {
        boardRepetition[boardString]++;
    }
    else {
        boardRepetition[boardString] = 1;
    }
    if (boardRepetition[boardString] >= 3) {
        isGameDrawn = true; // Draw by repetition
    }
    fiftyMoveRuleCounter++; // Increment the fifty-move rule counter
    if (fiftyMoveRuleCounter >= 50) {
        isGameDrawn = true;
    }
    checkInsufficientMaterial();
}

/**
 * Simulate a move on the chessboard without actually moving the piece.
 * It checks if the move would cause checkmate.
 * @param source_row The row of the source position
 * @param source_col The column of the source position
 * @param target_row The row of the target position
 * @param target_col The column of the target position
 * @param localBoard A reference to the local board state to simulate the move on
 * @return 0 if the move is valid and does not cause checkmate,
 * 31 if the move would cause checkmate.
 */
int ChessBoard::simulateMoveForCheck(const int source_row, const int source_col, const int target_row, const int target_col,
    vector<vector<Piece*>>& localBoard) const {
    // Simulate the move
    Piece* temp = localBoard[target_row][target_col];
    localBoard[target_row][target_col] = localBoard[source_row][source_col];
    localBoard[source_row][source_col] = nullptr;
    localBoard[target_row][target_col]->setRow(target_row);
    localBoard[target_row][target_col]->setCol(target_col);

    // Check if the move causes checkmate
    if (isKingInCheck(isWhiteTurn, localBoard)) {
        // Revert the move
        localBoard[source_row][source_col] = localBoard[target_row][target_col];
        localBoard[target_row][target_col] = temp;
        localBoard[source_row][source_col]->setRow(source_row);
        localBoard[source_row][source_col]->setCol(source_col);

        return 31; // This movement will cause checkmate
    }

    // Revert the move
    localBoard[source_row][source_col] = localBoard[target_row][target_col];
    localBoard[target_row][target_col] = temp;
    localBoard[source_row][source_col]->setRow(source_row);
    localBoard[source_row][source_col]->setCol(source_col);

    return 0; // The move is valid and does not cause checkmate
}


/**
 * Function to get all valid moves for a given piece.
 * @param piece the piece to get valid moves for
 * @param localBoard A reference to the local board state to check for valid moves
 * @return A vector of strings representing the valid moves in the format "a1b2"
 */
vector<string> ChessBoard::getValidMovesForPiece(const Piece *piece,
                                                 vector<vector<Piece*>>& localBoard) const  {
    if (piece == nullptr) {
        return vector<string>();
    }
    const int row = piece->getRow();
    const int col = piece->getCol();
    const bool forWhite = piece->getColor();

    vector<string> validMoves;
    for (int trg_row = 0; trg_row < 8; ++trg_row) {
        for (int trg_col = 0; trg_col < 8; ++trg_col) {
            const int code = isValidMove(row, col, trg_row, trg_col,localBoard,forWhite);
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
    return validMoves;
}


/**
 * Get all valid moves for a given color.
 * @param forWhite true if getting moves for white, false if for black
 * @param localBoard A reference to the local board state to check for valid moves
 * @return A vector of strings representing the valid moves in the format "a1b2"
 */
vector<string> ChessBoard::getAllValidMoves(const bool forWhite, vector<vector<Piece*>>& localBoard) const  {

    vector<string> validMoves;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (localBoard[row][col] != nullptr) {
                if (localBoard[row][col]->getColor() == forWhite) {
                    for (int trg_row = 0; trg_row < 8; ++trg_row) {
                        for (int trg_col = 0; trg_col < 8; ++trg_col) {
                            const int code = isValidMove(row, col, trg_row, trg_col,localBoard, forWhite);
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

    return validMoves;
}

/**
 * Simulate a move for evaluation purposes.
 * @param moveString The move string in the format "a1b2"
 * @param localBoard A reference to the local board state to simulate the move on
 * @return A Move object representing the simulated move
 */
ChessBoard::Move ChessBoard::simulateMove(const string &moveString, vector<vector<Piece*>>& localBoard) const {
    int fromRow = moveString[0] - 'a';
    int fromCol = moveString[1] - '1';
    int toRow = moveString[2] - 'a';
    int toCol = moveString[3] - '1';

    Piece* moved = localBoard[fromRow][fromCol];
    Piece* captured = localBoard[toRow][toCol];

    localBoard[toRow][toCol] = moved;
    localBoard[fromRow][fromCol] = nullptr;
    moved->setRow(toRow);
    moved->setCol(toCol);

    return {fromRow, fromCol, toRow, toCol, 0, captured };
}

/**
 * Undo a simulated move.
 * @param move The Move object representing the move to undo
 * @param localBoard A reference to the local board state to undo the move on
 */
void ChessBoard::undoMove(const Move &move,vector<vector<Piece*>>& localBoard) const {
    Piece* moved = localBoard[move.target_row][move.target_col];
    localBoard[move.source_row][move.source_col] = moved;
    moved->setRow(move.source_row);
    moved->setCol(move.source_col);

    localBoard[move.target_row][move.target_col] = move.pieceCaptured;
}

/**
 * Evaluate a move based on various criteria.
 * @param move The move object
 * @param localBoard A reference to the local board state to evaluate the move on
 * @return An integer score representing the evaluation of the move
 */
int ChessBoard::evaluateMove(const Move &move, vector<vector<Piece*>> &localBoard) const {
    int score = 0;

    const int toRow = move.target_row;
    const int toCol = move.target_col;


    const Piece* piece = localBoard[toRow][toCol];

    const bool isWhite = piece->getColor();

    const int checkmate = checkmateCheck(!isWhite, localBoard);
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
    const vector<Piece*> threats = getThreatsBy(isWhite ,toRow, toCol,localBoard);
    for (Piece* target : threats) {
        if (target->getValue() > piece->getValue() && dynamic_cast<King*>(target) == nullptr) {
            score += (target->getValue() - piece->getValue()) / 10;
        }
    }

    // Deduct value if the piece is threatened by a lower value piece
    const vector<Piece*> threatenedBy = getThreatsOn(isWhite ,toRow, toCol,localBoard);
    for (const Piece* attacker : threatenedBy) {
        if (attacker->getValue() < piece->getValue()) {
            score -= (piece->getValue() - attacker->getValue()) / 10;
        }
    }

    if (isKingInCheck(!isWhite, localBoard)) {
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
 * @param alpha The alpha value for pruning
 * @param beta The beta value for pruning
 * @param localBoard A reference to the local board state to evaluate the moves on
 * @return The score of the best move
 */
int ChessBoard::minimax(const bool forWhite, const int depth,int alpha
    , const int beta,vector<vector<Piece*>>& localBoard) {
    if (depth == 0) return 0;

    vector<string> moves = getAllValidMoves(forWhite, localBoard);
    if (moves.empty()) return 0;

    int bestScore = INT_MIN;
    for (const string& move : moves) {
        const int toRow = move[2] - 'a';
        const int toCol = move[3] - '1';

        Move moveObj(move);
        if (localBoard[toRow][toCol] != nullptr) {
            moveObj.setCapturedPiece(localBoard[toRow][toCol]);
        }
        auto sim = simulateMove(move, localBoard);

        int score = evaluateMove(moveObj, localBoard);
        const int reply = minimax(!forWhite, depth - 1, alpha,beta,localBoard);
        undoMove(sim, localBoard);

        score -= reply;
        if (score > bestScore) bestScore = score;
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return bestScore;
}

/**
 * Print the 3 best moves from the priority queue.
 * It catches any exceptions related to the priority queue.
 */
void ChessBoard::printBestMoves() const {
    try {
        cout << "The best 3 moves are: " << endl;
        cout << pq << endl;
    }
    catch (const PriorityQueueException& e) {
        cout << e.what() << endl;
    }
}

/**
 * Function to update the priority queue with the best moves.
 * it uses multiple threads to evaluate the moves.
 * Each thread evaluates a different move.
 */
void ChessBoard::updatePriorityQueue() {
    pq.clear();
    {
        lock_guard lock(threadMutex);
        threadsCompleted = 0;
    }
    const bool forWhite = isWhiteTurn;
    // Collect all pieces and valid moves
    vector<tuple<int, int, vector<string>>> pieceMoves;
    {
        auto sharedBoard = cloneBoard();
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (sharedBoard[i][j] && sharedBoard[i][j]->getColor() == forWhite) {
                    vector<string> moves = getValidMovesForPiece(sharedBoard[i][j], sharedBoard);
                    if (!moves.empty()) {
                        pieceMoves.emplace_back(i, j, moves);
                    }
                }
            }
        }
        // Clean up shared board
        for (auto& row : sharedBoard)
            for (auto& p : row)
                delete p;
    }

    // Distribute work for threads
    vector<vector<tuple<int, int, string>>> threadWork(numThreads);
    int moveIdx = 0;
    for (const auto& [i, j, moves] : pieceMoves) {
        for (const string& move : moves) {
            threadWork[moveIdx % numThreads].emplace_back(i, j, move);
            moveIdx++;
        }
    }

    // Process moves in threads
    vector<PriorityQueue<Move>> threadQueues(numThreads);
    vector<thread> workers;

    for (int t = 0; t < numThreads; ++t) {
        workers.emplace_back([&, t]() {
            auto localBoard = cloneBoard();
            for (const auto& [i, j, move] : threadWork[t]) {
                Move moveObj(move);
                const int toRow = move[2] - 'a';
                const int toCol = move[3] - '1';

                if (localBoard[toRow][toCol]) {
                    moveObj.setCapturedPiece(localBoard[toRow][toCol]);
                }

                auto sim = simulateMove(move, localBoard);
                int score = evaluateMove(moveObj, localBoard);
                const int replyScore = minimax(!forWhite,depth - 1,INT_MIN,INT_MAX , localBoard);
                undoMove(sim, localBoard);

                score -= replyScore;
                moveObj.setValue(score);
                threadQueues[t].push(moveObj);

            }
            {
                lock_guard lock(threadMutex);
                threadsCompleted++;
                threadCondition.notify_one();
            }

            // Clean up
            for (auto& row : localBoard)
                for (const auto& p : row)
                    delete p;
        });
    }

    // Merge results
    for (auto& worker : workers) {
        if (worker.joinable()) worker.join();
    }

    for (auto& threadQueue : threadQueues) {
        while (!threadQueue.empty()) {
            pq.push(threadQueue.pull());
        }
    }
}


/**
 * Function to check for checkmate or stalemate.
 * @param forWhite True if checking for white, false if checking for black
 * @param localBoard The local board state to check for checkmate or stalemate
 * @return checkmate status: 1 if white lost, 2 if black lost, 3 for stalemate, 0 for no checkmate or stalemate.
 */
int ChessBoard::checkmateCheck(const bool forWhite, vector<vector<Piece*>>& localBoard) const {
    const vector<string> allMoves = getAllValidMoves(forWhite, localBoard);
    if (allMoves.empty()) {
        if (isKingInCheck(forWhite, localBoard)) {
            return forWhite ? 1 : 2; // 1: White lost, 2: Black lost
        }
        return 3; // Stalemate
    }
    return 0; // No checkmate or stalemate
}

/**
 * Get all pieces that threaten by a specific position.
 * @param forWhite true if getting threats for white, false if for black
 * @param row The row of the position to check
 * @param col The column of the position to check
 * @param localBoard A reference to the local board state to check for threats
 * @return A vector of Piece pointers that threaten by the specified position
 */
vector<Piece*> ChessBoard::getThreatsBy(const bool forWhite, const int row, const int col
    ,const vector<vector<Piece*>>& localBoard) const {
    vector<Piece*> piecesThreaten;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (localBoard[i][j] != nullptr && localBoard[i][j]->getColor() != forWhite) {
                if (localBoard[row][col]->isValidMove(i, j) && isPathClear( row, col, i, j, localBoard)) {
                    piecesThreaten.push_back(localBoard[i][j]);
                }
            }
        }
    }
    return piecesThreaten;
}

/**
 * Get all pieces that threaten a specific position.
 * @param forWhite true if getting threats for white, false if for black
 * @param row The row of the position to check
 * @param col The column of the position to check
 * @param localBoard A reference to the local board state to check for threats
 * @return A vector of Piece pointers that threaten the specified position
 */
vector<Piece *> ChessBoard::getThreatsOn(const bool forWhite, const int row, const int col
    , const vector<vector<Piece*>>& localBoard) const {
    vector<Piece *> piecesThreat;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (localBoard[i][j] != nullptr && localBoard[i][j]->getColor() != forWhite) {
                if (localBoard[i][j]->isValidMove(row, col) && isPathClear(i, j, row, col,localBoard)) {
                    piecesThreat.push_back(localBoard[i][j]);
                }
            }
        }
    }
    return piecesThreat;
}

/**
 * Get the best move from the priority queue.
 * @return The best move as a string in the format "a1b2"
 * @throws PriorityQueueException if the priority queue is empty
 */
string ChessBoard::getBestMove() {
    if (!pq.empty()) {
        const auto move = pq.pull();
        return move.getMoveString();
    }
    throw PriorityQueueException("Priority queue is empty, no best move available");
}

/**
 * Initialize the thread pool with a specified number of threads.
 * @param numThreads the number of threads to initialize
 */
void ChessBoard::initThreadPool(const int numThreads) {
    if (numThreads <= 0) {
        throw invalid_argument("Number of threads must be positive");
    }
    this->numThreads = numThreads;
}

/**
 * Wait for all threads to complete their work.
 * This function blocks until all threads have finished.
 */
void ChessBoard::waitForThreads() {
    unique_lock<mutex> lock(threadMutex);
    threadCondition.wait(lock, [this]() {
        return threadsCompleted == numThreads;
    });
}

/**
 * Check if the game is drawn due to insufficient material.
 * It checks if there are not enough pieces left on the board to checkmate.
 */
void ChessBoard::checkInsufficientMaterial() {
    int numWhiteKnights = 0;
    int numWhiteBishops = 0;
    int numBlackKnights = 0;
    int numBlackBishops = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessBoard[i][j] != nullptr) {
                if (dynamic_cast<Knight*>(chessBoard[i][j]) != nullptr) {
                    if (chessBoard[i][j]->getColor()) {
                        numWhiteKnights++;
                    } else {
                        numBlackKnights++;
                    }
                } else if (dynamic_cast<Bishop*>(chessBoard[i][j]) != nullptr) {
                    if (chessBoard[i][j]->getColor()) {
                        numWhiteBishops++;
                    } else {
                        numBlackBishops++;
                    }
                }
                else if (dynamic_cast<King*>(chessBoard[i][j]) != nullptr) {
                    // Kings are not counted for insufficient material

                } else {
                    // If any other piece is present, it's not insufficient material
                    return;
                }
            }
        }
    }
    if (numWhiteKnights + numWhiteBishops + numBlackKnights + numBlackBishops > 1) {
        isInsufficientMaterial = false;
    }

    else {
        isInsufficientMaterial = true;
    }

}

/**
 * Check if the game is drawn due to insufficient material or by repetition.
 * @return true if the game is drawn, false otherwise
 */
bool ChessBoard::getDrawnGame() const {
    return isGameDrawn || isInsufficientMaterial;
}

/**
 * Get the current state of the chessboard as a string.
 * @return The string representation of the chessboard
 */
string ChessBoard::getBoardString() const {
    string boardString;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (chessBoard[i][j] != nullptr) {
                boardString += chessBoard[i][j]->getPieceName();
            } else {
                boardString += '#';
            }
        }
    }
    return boardString;
}


/**
 * Get the current turn of the chessboard.
 * @return true if it is white's turn, false if it is black
 */
bool ChessBoard::getIsWhiteTurn() const {
    return isWhiteTurn;
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