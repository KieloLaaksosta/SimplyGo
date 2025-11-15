#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "Point.h"
#include "Board.h"

extern const long long ZOBRIST_KEYS[ARR_LEN];

namespace SimplyGo
{
    class GameState
    {
    private:
        Board *previousBoard;
        Board *currentBoard;
        std::vector<long long> previousHashes;

        bool isUniquePosition(long long newHash);

    public:
        bool tryPlace(Point point, bool isWhite);

        GameState();
        ~GameState();
    };
}

#endif