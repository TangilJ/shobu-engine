#ifndef SEARCH_H
#define SEARCH_H
#include "types.h"

enum class Win
{
    OwnWin,
    OpponentWin,
    GameOngoing
};

Win checkWin(State state);

#endif //SEARCH_H
