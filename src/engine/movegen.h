#ifndef SHOBU_MOVEGEN_H
#define SHOBU_MOVEGEN_H

#include "types.h"

template<Direction Direction>
constexpr Bitboard move(Bitboard board);

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
void generateAggressiveMoves(Quarter quarter,
                             Moves<Quarter> &moveOnes,
                             Moves<Quarter> &moveTwos);

template<Direction direction, Location passiveBoard, Location aggressiveBoard>
void generateMovesOnBoard(const Board &board, std::vector<Board> &boards);

template<Direction direction>
void generateMovesForDirection(Board board, std::vector<Board> &boards);

void generateAllMovesInPly(Board board, std::vector<Board> &boards);

#endif //SHOBU_MOVEGEN_H
