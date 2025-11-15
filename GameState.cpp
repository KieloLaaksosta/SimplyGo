#include <stack>
#include <bitset>
#include <cstring>
#include "GameState.h"

bool SimplyGo::GameState::isUniquePosition(long long newHash)
{
    for (int i = 0; i < previousHashes.size(); i++)
        if (previousHashes[i] == newHash)
            return false;

    return true;
}

bool SimplyGo::GameState::tryPlace(Point point, bool isWhite)
{
    if (!Board::isValidPoint(point))
        return false;

    previousBoard->copyFrom(currentBoard);
    long long newHash = previousHashes[previousHashes.size() - 1];
    if(!previousBoard->place(point, isWhite, newHash))
        return false;

    if(!isUniquePosition(newHash))
        return false;

    std::swap(currentBoard, previousBoard);
    previousHashes.push_back(newHash);
    return true;
}

SimplyGo::GameState::GameState()
{
    previousHashes.push_back(Board::EMPTY_BOARD_HASH);

    previousBoard = new Board();
    currentBoard = new Board();
}

SimplyGo::GameState::~GameState()
{
    previousHashes.push_back(Board::EMPTY_BOARD_HASH);

    delete previousBoard;
    delete currentBoard;
}
