#ifndef SHOBU_PRINT_H
#define SHOBU_PRINT_H

#include "types.h"

void printBits(Board board);

void print(Board board);

void print(State board);

void printTwoBoards(TwoBoards boards);

void printTwoBoards(Board ownLeft, Board ownRight, Board enemyLeft, Board enemyRight);

#endif //SHOBU_PRINT_H
