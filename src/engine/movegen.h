#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

bool isPassiveMoveLegal(Bitboard target,
                        Bitboard ownStones,
                        Bitboard enemyStones);

template<Direction Direction>
bool isAggressiveMoveLegal(Quarter quarter,
                           Bitboard stone);

template<Direction Direction>
Quarter applyAggressiveMove(Quarter quarter,
                            Bitboard stone);

template<Direction direction>
void generatePassiveMoves(Quarter quarter,
                          Moves<Bitboard> &moveOnes,
                          Moves<Bitboard> &moveTwos);

template<Direction direction>
void generateMovesForDirection(Board board, std::vector<State> &states);

void generateAllMovesInPly(Board board, std::vector<State> &states);

Board reverseBoard(const Board &board);

#endif //SHOBU_MOVEGEN_H
