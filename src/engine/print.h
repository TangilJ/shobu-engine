#ifndef SHOBU_PRINT_H
#define SHOBU_PRINT_H

#include "types.h"
#include <string>

std::string toBitString(Bitboard board);

void printBits(Bitboard board);

void print(Bitboard board);

void print(State board);

void printTwoBoards(Quarter boards);

void printTwoBoards(Bitboard ownLeft, Bitboard ownRight,
                    Bitboard enemyLeft, Bitboard enemyRight);

#endif //SHOBU_PRINT_H
