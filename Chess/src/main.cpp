// Chess 
#include "Chess.h"
#include "ChessBoard.h"
#include "InvalidMoveException.h"
#include "PriorityQueue.h"
#include <chrono>


/**
 * Function to play an automatic game of chess (8 moves).
 * @param depth The depth of the search for the search algorithm.
 * @param numThreads The number of threads to use for the search algorithm.
 */
void automaticGame(const int depth,const int numThreads) {
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
			a.setCodeResponse(codeResponse);
		}
		catch (runtime_error& e) {
			cout << "Invalid move: " << e.what() << endl;
			a.setCodeResponse(codeResponse);
		}
	}
}


/**
 * Function to test the time taken for the automatic game with different thread counts.
 * @param depth The depth of the search for the search algorithm.
 */
void testTimes(const int depth) {
	vector<int> numThreads = {2,4,8};
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
void regularGame(const int depth) {
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
				a.setCodeResponse(codeResponse);

				continue;
			}
			catch (invalid_argument& e) {
				cout << "Invalid move: " << e.what() << endl;
				a.setCodeResponse(codeResponse);
				continue;
			}
		}

		a.setCodeResponse(codeResponse);
		auto localBoard = b.cloneBoard();
		const bool isWhiteTurn = b.getIsWhiteTurn();
		auto allMoves = b.getAllValidMoves(isWhiteTurn, localBoard);
		if (allMoves.empty()) {
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
		}
	}
}


int main()
{
	cout << "Enter the depth of the search: (recommended 2-4, max 8)" << endl;
	int depth,gameMode;
	cin >> depth;
	cout << "Enter the game mode: 1 for regular game, 2 for automatic game" << endl;
	while (depth < 1 || depth > 8) {
        cout << "Invalid depth, please enter a value between 1 and 8." << endl;
        cin >> depth;
    }
	cin >> gameMode;
	while (gameMode != 1 && gameMode != 2 ) {
		cout << "Invalid game mode, please enter 1 for regular game or 2 for automatic game." << endl;
		cin >> gameMode;
	}
	if (gameMode == 1) {
		regularGame(depth);
    }
	else {
		testTimes(depth);
	}

	cout << endl << "Exiting " << endl;
	return 0;
}
