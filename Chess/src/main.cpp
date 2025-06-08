#include <iostream>
#include "Game.h"
using namespace std;

int main()
{
	cout << "Enter the depth of the search: (1-4)" << endl;
	int depth,gameMode;
	cin >> depth;
	cout << "Enter the game mode: 1 for two players game, 2 for against the computer, 3 for automatic game (test times)" << endl;
	while (depth < 1 || depth > 4) {
        cout << "Invalid depth, please enter a value between 1 and 4." << endl;
        cin >> depth;
    }
	cin >> gameMode;
	while (gameMode < 1 || gameMode > 3) {
		cout << "Invalid game mode, please Enter the game mode: 1 for two players game, 2 for against the computer, 3 for automatic game (test times)" << endl;

		cin >> gameMode;
	}
	const Game game(gameMode, depth);
	game.run();

	cout << endl << "Exiting " << endl;
	return 0;
}
