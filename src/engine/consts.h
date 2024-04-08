#ifndef SHOBU_CONSTS_H
#define SHOBU_CONSTS_H

#include "types.h"

// @formatter:off
constexpr Bitboard ownStartPieces   = 0b0000'0000'0000'1111;
constexpr Bitboard enemyStartPieces = 0b1111'0000'0000'0000;

constexpr Bitboard top              = 0b1111'0000'0000'0000;
constexpr Bitboard bottom           = 0b0000'0000'0000'1111;
constexpr Bitboard left             = 0b1000'1000'1000'1000;
constexpr Bitboard right            = 0b0001'0001'0001'0001;

constexpr Bitboard empty            = 0b0000'0000'0000'0000;
// @formatter:on

constexpr State startState = {
    {ownStartPieces, enemyStartPieces},
    {ownStartPieces, enemyStartPieces},
    {ownStartPieces, enemyStartPieces},
    {ownStartPieces, enemyStartPieces}
};

#endif //SHOBU_CONSTS_H
