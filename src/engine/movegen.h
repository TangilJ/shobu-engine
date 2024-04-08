#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

template<Direction Direction>
constexpr Bitboard move(Bitboard board);

bool passiveMoveAllowed(Bitboard target,
                        Bitboard ownStones,
                        Bitboard enemyStones);

template<Direction Direction>
bool isAggressiveMoveLegal(Quarterboard quarter,
                           Bitboard stone);

template<Direction Direction>
Quarterboard applyAggressiveMove(Quarterboard quarter,
                                 Bitboard stone);

template<Direction Direction>
void generatePassiveMoves(Quarterboard quarter,
                          Vec<Bitboard, 4> &moveOnes,
                          Vec<Bitboard, 4> &moveTwos);

template<Direction Direction>
void generateAggressiveMoves(Quarterboard quarter,
                             Vec<Quarterboard, 4> &moveOnes,
                             Vec<Quarterboard, 4> &moveTwos);

template<Direction Direction, Quarter passiveBoard, Quarter aggressiveBoard>
void generateMovesOnBoard(const State &state, std::vector<State> &states);

template<Direction Direction>
void generateMovesForDirection(State state, std::vector<State> &states);

void generateAllMovesInPly(State state, std::vector<State> &states);

#endif //SHOBU_MOVEGEN_H
