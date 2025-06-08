//
// Created by Erez on 06/06/2025.
//

#ifndef GAMERUNNER_H
#define GAMERUNNER_H

/**
 * Class Game is responsible for running the chess game in different modes.
 */
class Game {
private:
    int gameMode;
    int depth;
public:
    Game(int mode, int depth);
    void run() const;
    void regularGame(int depth) const;
    void testTimes(int depth) const;
    void automaticGame(int depth,int numThreads) const;
    void ai_game(int depth) const;
};



#endif //GAMERUNNER_H
