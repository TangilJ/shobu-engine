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
                          std::vector<Board> &moveOnes,
                          std::vector<Board> &moveTwos);

template<Direction Direction>
void generateAggressiveMoves(Board own,
                             Board enemy,
                             std::vector<BoardPair> &moveOnes,
                             std::vector<BoardPair> &moveTwos);

template<Direction Direction>
void plyForDirection(BoardType passiveBoard,
                     BoardType aggressiveBoard,
                     const State &state,
                     States &states);

template<Direction Direction>
void addMovesForDirection(State state, States &states);

void addMoves(State state, States &states);

#endif //SHOBU_MOVEGEN_H
