#include "search.h"
#include <immintrin.h>

// If any one of a player's 4x4 boards has no pieces left, the player loses
Win checkWin(const Board board)
{
    for (const Quarter quarter: {board.topLeft, board.topRight, board.bottomLeft, board.bottomRight})
    {
        if (quarter.own == 0)
            return Win::OpponentWin;
        if (quarter.enemy == 0)
            return Win::OwnWin;
    }

    return Win::GameOngoing;
}
