#include <stack>
#include <bitset>
#include "GameState.h"

SimplyGo::StoneFlags SimplyGo::GameState::getFlags(Point point)
{
    return stones[getIndex(point)];
}

void SimplyGo::GameState::setFlags(Point point, StoneFlags flags)
{
    stones[getIndex(point)] = flags;
}

void SimplyGo::GameState::initStones()
{
    for (int i = 0; i < ARR_LEN; i++)
        stones[i] = StoneFlags::None;
}

bool SimplyGo::GameState::wouldCapture(Point startingPoint, bool isPlayer1)
{
    std::bitset<ARR_LEN> visitedPoints = std::bitset<ARR_LEN>();
    std::stack<Point> workStack = std::stack<Point>();

    workStack.push(startingPoint);
    visitedPoints.reset();

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
            if (!(flags & StoneFlags::IsPlaced) && offsetPoint != startingPoint)
                return false;

            if ((flags & StoneFlags::IsPlayer1 == StoneFlags::IsPlayer1) == isPlayer1)
            {
                workStack.push(offsetPoint);
            }
        }
    }

    return true;
}

bool SimplyGo::GameState::capture(Point point, bool isPlayer1)
{
    std::stack<Point> workStack = std::stack<Point>();
    workStack.push(point);

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
            if ((flags & StoneFlags::IsPlayer1 == StoneFlags::IsPlayer1) != isPlayer1)
            {
                setFlags(offsetPoint, StoneFlags::None);
                workStack.push(offsetPoint);
            }
        }
    }

    return true;
}

void SimplyGo::GameState::place(Point point, bool isPlayer1)
{
    setFlags(point, (StoneFlags::IsPlaced | isPlayer1 ? StoneFlags::IsPlayer1 : StoneFlags::None));
    for (int i = 0; i < 4; i++)
    {
        Point offsetPoint = point + DIRECTIONS[i];
        StoneFlags flags = getFlags(offsetPoint);
        if (flags | StoneFlags::IsPlaced && (flags | StoneFlags::IsPlayer1 == StoneFlags::IsPlayer1) != isPlayer1)
        {
            if (wouldCapture(point, !isPlayer1))
                capture(offsetPoint, !isPlayer1);
        }
    }

    if (wouldCapture(point, isPlayer1))
    capture(point, isPlayer1);
}

bool SimplyGo::GameState::canPlace(Point point, bool isPlayer1)
{
    if (!isValidPoint(point))
        return false;

    for (int i = 0; i < 4; i++)
    {
        Point offsetPoint = point + DIRECTIONS[i];
        StoneFlags flags = getFlags(offsetPoint);
        if (flags | StoneFlags::IsPlaced && (flags | StoneFlags::IsPlayer1 == StoneFlags::IsPlayer1) != isPlayer1)
        {
            if (wouldCapture(point, !isPlayer1))
                return true;
        }
    }

    return wouldCapture(point, isPlayer1);
}

bool SimplyGo::GameState::isValidPoint(Point point)
{
    return point.x >= 0 && point.y >= 0 && point.x < BOARD_SIZE && point.y < BOARD_SIZE;
}

int SimplyGo::GameState::getIndex(Point point)
{
    return point.x + BOARD_SIZE * point.y;
}

bool SimplyGo::GameState::tryPlace(Point point, bool isPlayer1)
{
    if (!canPlace(point, isPlayer1))
        return false;

    place(point, isPlayer1);

    return true;
}

SimplyGo::GameState::GameState()
{
    initStones();
}
