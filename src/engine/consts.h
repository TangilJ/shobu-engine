#ifndef SHOBU_CONSTS_H
#define SHOBU_CONSTS_H

#include "types.h"

// @formatter:off
constexpr Board ownStartPieces   = 0b0000'0000'0000'1111;
constexpr Board enemyStartPieces = 0b1111'0000'0000'0000;

constexpr Board top              = 0b1111'0000'0000'0000;
constexpr Board bottom           = 0b0000'0000'0000'1111;
constexpr Board left             = 0b1000'1000'1000'1000;
constexpr Board right            = 0b0001'0001'0001'0001;

constexpr Board empty            = 0b0000'0000'0000'0000;
// @formatter:on

constexpr State startState = {
    {ownStartPieces, ownStartPieces, ownStartPieces, ownStartPieces},
    {enemyStartPieces, enemyStartPieces, enemyStartPieces, enemyStartPieces}
};

#endif //SHOBU_CONSTS_H
