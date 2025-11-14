#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Point.h"

#define BOARD_SIZE 19
#define ARR_LEN 19 * 19

namespace SimplyGo
{
    enum StoneFlags : char
    {
        None = 0,
        IsPlaced = 1,
        IsPlayer1 = 2
    };

    class GameState
    {
    private:
        StoneFlags stones[ARR_LEN];

        StoneFlags getFlags(Point point);
        void setFlags(Point point, StoneFlags flags);

        void initStones();

        bool wouldKill(Point point, bool isPlayer1);
        bool kill(Point point, bool isPlayer1);

        void place(Point point, bool isPlayer1);
        bool canPlace(Point point, bool isPlayer1);

        bool isValidPoint(Point point);
        int getIndex(Point point);

    public:
        bool tryPlace(Point point, bool isPlayer1);
        
        GameState();
    };
}

#endif