//
// Created by Erez on 06/06/2025.
//

#include "Game.h"
#include "Chess.h"
#include "ChessBoard.h"
#include "InvalidMoveException.h"
#include "PriorityQueue.h"
#include <chrono>

/**
 * Constructor for the Game class.
 */
Game::Game(const int mode, const int depth): gameMode(mode), depth(depth) {}

/**
 * Function to run the game based on the selected mode.
 * If mode is 1, it runs a regular game.
 * If mode is 2, it runs an AI game.
 * If mode is 3, it tests the time taken for automatic games with different thread counts.
 */
void Game::run() const {
	if (gameMode == 1) {
		regularGame(depth);
	} else if (gameMode == 2) {
		ai_game(depth);
	} else if (gameMode == 3) {
		testTimes(depth);
	}
}

/**
 * Function to play a game against the AI.
 * It allows the user to choose their color and plays the game until a checkmate, stalemate, or draw occurs.
 * @param depth The depth of the search for the search algorithm.
 */
void Game::ai_game(const int depth) const {
	cout << "Enter w for white or b for black" << endl;
	char playerColor;
	cin >> playerColor;
	while (playerColor != 'w' && playerColor != 'b') {
        cout << "Invalid color, please enter w for white or b for black" << endl;
        cin >> playerColor;
    }

	const string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
	constexpr int numThreads = 8;
	Chess a(board);
	ChessBoard b(board, depth);

	int codeResponse = 0;
	b.initThreadPool(numThreads);
	if (playerColor == 'b') {
		b.updatePriorityQueue();
		b.waitForThreads();
        const string move = b.getBestMove();
		codeResponse = b.executeMove(move);
		a.setCodeResponse(codeResponse);
		a.getInput(move);
    }
	string res;
	while (res != "exit")
	{
		{
			b.updatePriorityQueue();
			b.waitForThreads();
			b.printBestMoves();
			res = a.getInput();
			try {
				codeResponse = b.executeMove(res);
				a.setCodeResponse(codeResponse);
			}
			catch (InvalidMoveException& e) {
				cout << "Invalid move: " << e.what() << endl;
				a.setCodeResponse(e.getErrorCode());

				continue;
			}
			catch (invalid_argument& e) {
				cout << "Invalid move: " << e.what() << endl;
				continue;
			}
		}
		auto localBoard = b.cloneBoard();
		bool isWhiteTurn = b.getIsWhiteTurn();

		int checkmateCode = b.checkmateCheck(isWhiteTurn, localBoard);
		if (checkmateCode == 3) {
			cout << "Stalemate!" << endl;
			break;
		}
		if (isWhiteTurn && checkmateCode == 1) {
			cout << "White lost!" << endl;
			break;
		}
		if (checkmateCode == 2) {
			cout << "Black lost!" << endl;
			break;

		}
		if (b.getDrawnGame()) {
			cout << "The game ended in a draw!" << endl;
            break;
		}
		b.updatePriorityQueue();
		b.waitForThreads();
		string bestMove = b.getBestMove();
		codeResponse = b.executeMove(bestMove);
		a.getInput(bestMove);
		a.setCodeResponse(codeResponse);
		localBoard = b.cloneBoard();
		isWhiteTurn = b.getIsWhiteTurn();

		checkmateCode = b.checkmateCheck(isWhiteTurn, localBoard);
		if (checkmateCode == 3) {
			cout << "Stalemate!" << endl;
			break;
		}
		if (isWhiteTurn && checkmateCode == 1) {
			cout << "White lost!" << endl;
			break;
		}
		if (checkmateCode == 2) {
			cout << "Black lost!" << endl;
			break;

		}
		if (b.getDrawnGame()) {
			cout << "The game ended in a draw!" << endl;
			break;
		}
	}
}


/**
 * Function to play an automatic game of chess (8 moves).
 * @param depth The depth of the search for the search algorithm.
 * @param numThreads The number of threads to use for the search algorithm.
 */
void Game::automaticGame(const int depth,const int numThreads) const {
	const string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
	Chess a(board);
	ChessBoard b(board, depth);
	int numMoves = 0;
	int codeResponse = 0;
	b.initThreadPool(numThreads);

	while (numMoves < 8) {

		try {
			b.updatePriorityQueue();
			b.waitForThreads();
			string move = b.getBestMove();
			codeResponse = b.executeMove(move);
			numMoves++;
			a.setCodeResponse(codeResponse);
		}
		catch (InvalidMoveException& e) {
			cout << "Invalid move: " << e.what() << endl;
			a.setCodeResponse(e.getErrorCode());
		}
		catch (runtime_error& e) {
			cout << "Invalid move: " << e.what() << endl;
		}
	}
}


/**
 * Function to test the time taken for the automatic game with different thread counts.
 * @param depth The depth of the search for the search algorithm.
 */
void Game::testTimes(const int depth) const {
	vector<int> numThreads = {1,2,4,8};
	for (const int threads : numThreads) {
        auto start = chrono::high_resolution_clock::now();
        automaticGame(depth, threads);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Time taken with " << threads << " threads: " << duration.count() << " ms" << endl;
    }
}


/**
 * Function to play a regular game of chess.
 * @param depth The depth of the search for the search algorithm.
 */
void Game::regularGame(const int depth) const {
    const string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
	constexpr int numThreads = 8;
	Chess a(board);
	ChessBoard b(board, depth);

	int codeResponse = 0;
	b.initThreadPool(numThreads);

	string res;
	while (res != "exit")
	{
		/*
		codeResponse value :
		Illegal movements :
		11 - there is not piece at the source
		12 - the piece in the source is piece of your opponent
		13 - there one of your pieces at the destination
		21 - illegal movement of that piece
		31 - this movement will cause you checkmate

		legal movements :
		41 - the last movement was legal and cause check
		42 - the last movement was legal, next turn
		*/
		{
			b.updatePriorityQueue();
			b.waitForThreads();
			b.printBestMoves();
			res = a.getInput();
			try {
				codeResponse = b.executeMove(res);
			}
			catch (InvalidMoveException& e) {
				cout << "Invalid move: " << e.what() << endl;
				a.setCodeResponse(e.getErrorCode());
				continue;
			}
			catch (invalid_argument& e) {
				cout << "Invalid move: " << e.what() << endl;
				continue;
			}
		}

		a.setCodeResponse(codeResponse);
		auto localBoard = b.cloneBoard();
		const bool isWhiteTurn = b.getIsWhiteTurn();

		const int checkmateCode = b.checkmateCheck(isWhiteTurn, localBoard);
		if (checkmateCode == 3) {
			cout << "Stalemate!" << endl;
			break;
		}
		if (isWhiteTurn && checkmateCode == 1) {
			cout << "White lost!" << endl;
			break;
		}
		if (checkmateCode == 2) {
			cout << "Black lost!" << endl;
			break;

		}
		if (b.getDrawnGame()) {
			cout << "The game ended in a draw!" << endl;
			break;
		}
	}
}
