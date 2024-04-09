#ifndef SEARCH_H
#define SEARCH_H

#include "types.h"

enum class Win
{
    OwnWin,
    OpponentWin,
    GameOngoing
};

Win checkWin(Board board);

#endif //SEARCH_H
