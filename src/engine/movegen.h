#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

template<Direction Direction>
constexpr Board move(Board board);

bool passiveMoveAllowed(Board target,
                        Board ownStones,
                        Board enemyStones);

template<Direction Direction>
BoardPair aggressiveMove(Board ownStones,
                         Board enemyStones,
                         Board stone);

template<Direction Direction>
void generatePassiveMoves(Board own,
                          Board enemy,
                          Vec<Board, 4> &moveOnes,
                          Vec<Board, 4> &moveTwos);

template<Direction Direction>
void generateAggressiveMoves(Board own,
                             Board enemy,
                             Vec<BoardPair, 4> &moveOnes,
                             Vec<BoardPair, 4> &moveTwos);

template<Direction Direction>
void generateMovesOnBoard(BoardType passiveBoard,
                          BoardType aggressiveBoard,
                          const State &state,
                          std::vector<State> &states);

template<Direction Direction>
void generateMovesForDirection(State state, std::vector<State> &states);

void generateAllMovesInPly(State state, std::vector<State> &states);

#endif //SHOBU_MOVEGEN_H
