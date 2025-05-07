// Chess 
#include "Chess.h"
#include "ChessBoard.h"
#include "InvalidMoveException.h"
#include "PriorityQueue.h"

int main()
{
	const string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";

	Chess a(board);
	ChessBoard b(board);
	PriorityQueue<ChessBoard::Move> pq;

	int codeResponse = 0;
	bool isWhiteTurn = true;

	vector<ChessBoard::Move> allMovesScores = b.getAllMovesScores(isWhiteTurn);
	for (const auto& move : allMovesScores) {
		pq.push(move);
	}
	try {
		cout << "The best 3 moves are: " << endl;
		cout << pq << endl;
	}
	catch (PriorityQueueException& e) {
		cout << "Error: " << e.what() << endl;
		return 1;
	}
	string res = a.getInput();
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
			try {
				codeResponse = b.executeMove(res);
				pq.clear();
				isWhiteTurn = !isWhiteTurn;
			}
			catch (InvalidMoveException& e) {
				cout << "Invalid move: " << e.what() << endl;
				a.setCodeResponse(codeResponse);
				res = a.getInput();
				continue;
			}
		}

		a.setCodeResponse(codeResponse);

		allMovesScores = b.getAllMovesScores(isWhiteTurn);
		if (allMovesScores.empty()) {
			const int checkmateCode = b.checkmateCheck(isWhiteTurn);
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

		for (const auto& move : allMovesScores) {
			pq.push(move);
		}
		try {
			cout << "The best 3 moves are: " << endl;
			cout << pq << endl;
		}
		catch (PriorityQueueException& e) {
			cout << "Error: " << e.what() << endl;
			return 1;
		}

		res = a.getInput();
	}

	cout << endl << "Exiting " << endl;
	return 0;
}
