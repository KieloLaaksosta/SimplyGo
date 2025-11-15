#include <stack>
#include <bitset>
#include <cstring>
#include <random>
#include <iostream>
#include "Board.h"

const std::array<long long, SimplyGo::Board::KEY_COUNT> SimplyGo::Board::ZOBRIST_KEYS = []()
{
    constexpr unsigned int FIXED_SEED = 0;
    std::mt19937_64 engine(FIXED_SEED);
    std::uniform_int_distribution<long long> distribution;

    std::array<long long, KEY_COUNT> keys{};

    for (int i = 0; i < KEY_COUNT; i++)
    {
        keys[i] = distribution(engine);
    }

    return keys;
}();

const long long SimplyGo::Board::EMPTY_BOARD_HASH = []()
{
    long long hash = 0;
    for (int i = 0; i < KEY_COUNT; i += 3)
    {
        hash ^= ZOBRIST_KEYS[i];
    }
    return hash;
}();

long long SimplyGo::Board::getStoneHash(Point point, StoneFlags flags)
{
    int offset;
    switch (flags)
    {
    case StoneFlags::IsPlaced | StoneFlags::IsWhite:
        offset = 2;
        break;
    case StoneFlags::IsPlaced & ~StoneFlags::IsWhite:
        offset = 1;
        break;
    default:
        offset = 0;
        break;
    }

    return ZOBRIST_KEYS[getIndex(point) + offset];
}

SimplyGo::StoneFlags SimplyGo::Board::getFlags(Point point) const
{
    return stones[getIndex(point)];
}

void SimplyGo::Board::setFlags(Point point, StoneFlags flags)
{
    stones[getIndex(point)] = flags;
}

bool SimplyGo::Board::isCaptured(Point startingPoint, bool isWhite) const
{
    std::bitset<ARR_LEN> visitedPoints = std::bitset<ARR_LEN>();
    std::stack<Point> workStack = std::stack<Point>();

    workStack.push(startingPoint);

    while (!workStack.empty())
    {
        Point point = workStack.top();
        workStack.pop();

        int index = getIndex(point);
        if (visitedPoints[index])
            continue;
        visitedPoints.set(index);

        for (int i = 0; i < 4; i++)
        {
            Point offsetPoint = point + DIRECTIONS[i];

            if (!isValidPoint(offsetPoint))
                continue;

            StoneFlags flags = getFlags(offsetPoint);
            if (!(flags & StoneFlags::IsPlaced))
                return false;

            if (((flags & StoneFlags::IsWhite) == StoneFlags::IsWhite) == isWhite)
                workStack.push(offsetPoint);
        }
    }

    return true;
}

bool SimplyGo::Board::capture(Point point, bool isWhite, long long &newHash)
{
    std::stack<Point> workStack = std::stack<Point>();
    workStack.push(point);
    newHash ^= getStoneHash(point, getFlags(point));
    newHash ^= getStoneHash(point, StoneFlags::None);
    setFlags(point, StoneFlags::None);

    while (!workStack.empty())
    {
        point = workStack.top();
        workStack.pop();

        for (int i = 0; i < 4; i++)
        {
            Point offsetPoint = point + DIRECTIONS[i];
            if (!isValidPoint(offsetPoint))
                continue;

            StoneFlags flags = getFlags(offsetPoint);
            if (flags & StoneFlags::IsPlaced && ((flags & StoneFlags::IsWhite) == StoneFlags::IsWhite) == isWhite)
            {
                newHash ^= getStoneHash(offsetPoint, flags);
                newHash ^= getStoneHash(offsetPoint, StoneFlags::None);
                setFlags(offsetPoint, StoneFlags::None);
                workStack.push(offsetPoint);
            }
        }
    }

    return true;
}

bool SimplyGo::Board::isValidPoint(Point point)
{
    return point.x >= 0 && point.y >= 0 && point.x < BOARD_SIZE && point.y < BOARD_SIZE;
}

int SimplyGo::Board::getIndex(Point point)
{
    return point.x + BOARD_SIZE * point.y;
}

bool SimplyGo::Board::place(Point point, bool isWhite, long long &newHash)
{
    newHash ^= getStoneHash(point, getFlags(point));

    StoneFlags newFlags = (StoneFlags)(isWhite ? (StoneFlags::IsPlaced | StoneFlags::IsWhite) : (StoneFlags::IsPlaced & ~StoneFlags::IsWhite));
    setFlags(point, newFlags);
    newHash ^= getStoneHash(point, newFlags);

    bool isLegal = false;

    for (int i = 0; i < 4; i++)
    {
        Point offsetPoint = point + DIRECTIONS[i];
        StoneFlags flags = getFlags(offsetPoint);
        if ((flags & StoneFlags::IsPlaced) && ((flags & StoneFlags::IsWhite) == StoneFlags::IsWhite) != isWhite)
        {
            if (isCaptured(offsetPoint, !isWhite))
            {
                capture(offsetPoint, !isWhite, newHash);
                isLegal = true;
            }
        }
    }

    if (isCaptured(point, isWhite))
        capture(point, isWhite, newHash);
    else
        isLegal = true;

    return isLegal;
}

void SimplyGo::Board::copyFrom(const Board *board)
{
    std::copy(std::begin(board->stones), std::end(board->stones), stones);
}

void SimplyGo::Board::printBoard() const
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        std::cout.width(2);
        std::cout << std::right << i;

        for (int j = 0; j < BOARD_SIZE; j++)
        {
            int index = getIndex(Point(i, j));

            StoneFlags flags = stones[index];
            char displayChar = '.';

            if (flags & StoneFlags::IsPlaced)
            {
                if (flags & StoneFlags::IsWhite)
                {
                    displayChar = 'O';
                }
                else
                {
                    displayChar = 'X';
                }
            }

            std::cout << " " << displayChar;
        }
        std::cout << "\n";
    }
    std::cout << "\n\n" << std::flush;
}

SimplyGo::Board::Board()
{
    std::fill_n(stones, ARR_LEN, StoneFlags::None);
}