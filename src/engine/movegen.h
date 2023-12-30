#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

void addPassiveMoves(BoardType passiveBoard,
                     const State &state,
                     States &states);

TwoBoards applyStoneAggressiveMove(Board ownAggrStones,
                                   Board enemyAggrStones,
                                   Board stone);

void addBothMoves(State state,
                  BoardType aggressiveBoard,
                  BoardType passiveBoard,
                  States &states);

void addMoves(State state, States &states);

#endif //SHOBU_MOVEGEN_H
