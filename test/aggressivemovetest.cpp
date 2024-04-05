#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "../src/engine/movegen.h"
#include "../src/engine/consts.h"

template <Direction Direction>
void checkLegal(const Bitboard own, const Bitboard enemy, const Bitboard stone,
                const Bitboard ownExpected, const Bitboard enemyExpected)
{
    const Quarterboard result = aggressiveMove<Direction>(
        own, enemy, stone
    );
    CHECK((ownExpected == result.own));
    CHECK((enemyExpected == result.enemy));
}


template <Direction Direction>
void checkIllegal(const Bitboard own, const Bitboard enemy, const Bitboard stone)
{
    const Quarterboard result = aggressiveMove<Direction>(
        own, enemy, stone
    );
    CHECK((0 == result.own));
    CHECK((0 == result.enemy));
}

#pragma region Up
TEST_CASE("Aggressive move up - 1 own, 0 enemy")
{
    checkLegal<Direction::Up>(0b0000'0000'0100'0000, empty,
                              0b0000'0000'0100'0000,
                              0b0000'0100'0000'0000, empty);
}

TEST_CASE("Aggressive move up - many own, 0 enemy")
{
    checkLegal<Direction::Up>(0b0110'1010'0100'0110, empty,
                              0b0000'0000'0100'0000,
                              0b0110'1110'0000'0110, empty);
}

TEST_CASE("Aggressive move up - 1 own, pushing 1 enemy")
{
    checkLegal<Direction::Up>(0b0000'0000'0000'1000, 0b0000'0000'1000'0000,
                              0b0000'0000'0000'1000, 0b0000'0000'1000'0000,
                              0b0000'1000'0000'0000);
}

TEST_CASE("Aggressive move up - many own, pushing 1 enemy of many")
{
    checkLegal<Direction::Up>(0b0110'1000'0100'0110, 0b0000'0000'0010'1001,
                              0b0000'0000'0000'0010, 0b0110'1000'0110'0100,
                              0b0000'0010'0000'1001);
}

TEST_CASE("Illegal aggressive move up - on edge")
{
    checkIllegal<Direction::Up>(0b0100'0000'0000'0000,
                                empty,
                                0b0100'0000'0000'0000);
}

TEST_CASE("Illegal aggressive move up - pushing 2 enemy stones")
{
    checkIllegal<Direction::Up>(0b0000'0000'0010'0000,
                                0b0010'0010'0000'0000,
                                0b0000'0000'0010'0000);
}

TEST_CASE("Illegal aggressive move up - pushing enemy stone into own")
{
    checkIllegal<Direction::Up>(0b0010'0000'0010'0000,
                                0b0000'0010'0000'0000,
                                0b0000'0000'0010'0000);
}


TEST_CASE("Illegal aggressive move up - pushing own stone")
{
    checkIllegal<Direction::Up>(0b0000'0010'0010'0000,
                                empty,
                                0b0000'0000'0010'0000);
}
#pragma endregion

#pragma region DownLeft
TEST_CASE("Aggressive move down left - 1 own, 1 enemy")
{
    checkLegal<Direction::DownLeft>(0b0000'0010'0000'0000,
                                    0b0000'0000'0100'0000,
                                    0b0000'0010'0000'0000,
                                    0b0000'0000'0100'0000,
                                    0b0000'0000'0000'1000);
}
#pragma endregion


#pragma region Down
TEST_CASE("Aggressive move down - 1 own, 1 enemy, pushing enemy off board")
{
    checkLegal<Direction::Down>(0b0000'0000'0010'0000,
                                    0b0000'0000'0000'0010,
                                    0b0000'0000'0010'0000,
                                    0b0000'0000'0000'0010,
                                    empty);
}
#pragma endregion


#pragma region Right
TEST_CASE("Aggressive move right - 1 own, 1 enemy, pushing enemy off board")
{
    checkLegal<Direction::Right>(0b0000'0000'0010'0000,
                                    0b0000'0000'0001'0000,
                                    0b0000'0000'0010'0000,
                                    0b0000'0000'0001'0000,
                                    empty);
}
#pragma endregion
