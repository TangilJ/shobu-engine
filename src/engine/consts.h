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
constexpr Board topHalf          = 0b1111'1111'0000'0000;
constexpr Board bottomHalf       = 0b0000'0000'1111'1111;
constexpr Board leftHalf         = 0b1100'1100'1100'1100;
constexpr Board rightHalf        = 0b0011'0011'0011'0011;
// @formatter:on

#endif //SHOBU_CONSTS_H
