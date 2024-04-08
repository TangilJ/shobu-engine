#include "search.h"
#include <immintrin.h>

// If any one of a player's 4x4 boards has no pieces left, the player loses
Win checkWin(const State state)
{
    for (const Quarter board: {state.topLeft, state.topRight, state.bottomLeft, state.bottomRight})
    {
        if (board.own == 0)
            return Win::OpponentWin;
        if (board.enemy == 0)
            return Win::OwnWin;
    }

    return Win::GameOngoing;
}
