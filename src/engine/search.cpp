#include "search.h"
#include <immintrin.h>

// If any one of a player's 4x4 boards has no pieces left, the player loses
Win checkWin(const State state)
{
    for (const Bitboard board: state.own)
        if (_mm_popcnt_u32(board) == 0)
            return Win::OpponentWin;

    for (const Bitboard board: state.enemy)
        if (_mm_popcnt_u32(board) == 0)
            return Win::OwnWin;

    return Win::GameOngoing;
}
