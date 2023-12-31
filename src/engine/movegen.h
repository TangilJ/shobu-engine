#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

template <Direction>
void addPassiveMoves(BoardType passiveBoard,
                     const State& state,
                     States& states);

template <Direction Direction>
TwoBoards applyStoneAggressiveMove(Board ownStones,
                                   Board enemyStones,
                                   Board stone);

template <Direction Direction>
void addBothMoves(State state,
                  BoardType aggressiveBoard,
                  BoardType passiveBoard,
                  States& states);

template <Direction Direction>
void addMovesForDirection(State state, States& states);

void addMoves(State state, States& states);

#endif //SHOBU_MOVEGEN_H
