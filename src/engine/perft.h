#ifndef PERFT_H
#define PERFT_H

#include <cstdint>

#include "types.h"

uint64_t perft(Board board, int depth);

void timeIncreasingPerft(int depth);

#endif //PERFT_H
