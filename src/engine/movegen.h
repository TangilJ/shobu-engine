#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

template<Direction Direction>
constexpr Bitboard move(Bitboard board);

bool passiveMoveAllowed(Bitboard target,
                        Bitboard ownStones,
                        Bitboard enemyStones);

template<Direction Direction>
Quarterboard aggressiveMove(Bitboard ownStones,
                            Bitboard enemyStones,
                            Bitboard stone);

template<Direction Direction>
void generatePassiveMoves(Bitboard own,
                          Bitboard enemy,
                          Vec<Bitboard, 4> &moveOnes,
                          Vec<Bitboard, 4> &moveTwos);

template<Direction Direction>
void generateAggressiveMoves(Bitboard own,
                             Bitboard enemy,
                             Vec<Quarterboard, 4> &moveOnes,
                             Vec<Quarterboard, 4> &moveTwos);

template<Direction Direction>
void generateMovesOnBoard(Quarter passiveBoard,
                          Quarter aggressiveBoard,
                          const State &state,
                          std::vector<State> &states);

template<Direction Direction>
void generateMovesForDirection(State state, std::vector<State> &states);

void generateAllMovesInPly(State state, std::vector<State> &states);

#endif //SHOBU_MOVEGEN_H
