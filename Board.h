#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <array>
#include "Point.h"

constexpr int BOARD_SIZE = 19;
constexpr int ARR_LEN = 19 * 19;

namespace SimplyGo
{
    enum StoneFlags : char
    {
        None = 0,
        IsPlaced = 1,
        IsWhite = 2
    };

    class Board
    {
    private:
        static constexpr int KEY_COUNT = 3 * ARR_LEN;
        static const std::array<long long, KEY_COUNT> ZOBRIST_KEYS;
        static long long getStoneHash(Point point, StoneFlags flags);

        StoneFlags stones[ARR_LEN];

        StoneFlags getFlags(Point point) const;
        void setFlags(Point point, StoneFlags flags);

        bool isCaptured(Point startingPoint, bool isWhite) const;
        bool capture(Point point, bool isWhite, long long &newHash);

    public:
        static const long long EMPTY_BOARD_HASH;

        static bool isValidPoint(Point point);
        static int getIndex(Point point);

        bool place(Point point, bool isWhite, long long &newHash);

        void copyFrom(const Board *board);

        Board();
    };
}

#endif