#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

void addPassiveMoves(BoardType passiveBoard, 
                     const State &state, 
                     States &states);

TwoBoards addStoneAggressiveMove(State state, Board stone,
                                 BoardType aggressiveBoard,
                                 BoardType passiveBoard,
                                 States &states);

void addAggressiveMoves(State state,
                        BoardType aggressiveBoard,
                        BoardType passiveBoard,
                        States &states);

void addMoves(State state, States &states);

#endif //SHOBU_MOVEGEN_H
