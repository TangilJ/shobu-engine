#include <doctest/doctest.h>
#include <algorithm>
#include "../src/engine/movegen.h"
#include "../src/engine/types.h"
#include "../src/engine/consts.h"


#pragma region Up

TEST_CASE("Passive move up - one stone")
{
    CHECK(
        passiveMoveAllowed(
            0b0000'0000'1000'0000,
            0b0000'0000'0000'1000,
            empty
        )
    );
}

static void checkContains(Vec<Bitboard, 4> &moveOnes, Bitboard b)
{
    CHECK((std::find(moveOnes.begin(), moveOnes.end(), b) != moveOnes.end()));
}

template<Direction Direction>
void checkPassiveMoves(Bitboard own,
                       Bitboard enemy,
                       const std::vector<Bitboard> &expectedOnes,
                       const std::vector<Bitboard> &expectedTwos)
{
    Vec<Bitboard, 4> moveOnes;
    Vec<Bitboard, 4> moveTwos;
    generatePassiveMoves<Direction>(
        own,
        enemy,
        moveOnes,
        moveTwos
    );

    CHECK((expectedOnes.size() == moveOnes.size()));
    for (const Bitboard &b: expectedOnes)
        checkContains(moveOnes, b);


    CHECK((expectedTwos.size() == moveTwos.size()));
    for (const Bitboard &b: expectedTwos)
        checkContains(moveTwos, b);
}

TEST_CASE("Passive move up - four stones")
{
    checkPassiveMoves<Direction::Up>(
        0b0000'0001'0010'1100,
        empty,
        {
            0b0000'0001'0110'1000,
            0b0000'0001'1010'0100,
            0b0000'0011'0000'1100,
            0b0001'0000'0010'1100
        },
        {
            0b0010'0001'0000'1100,
            0b0000'1001'0010'0100,
            0b0000'0101'0010'1000
        }
    );
}

TEST_CASE("Passive move up - surrounded by enemy stones")
{
    checkPassiveMoves<Direction::Up>(
        0b0000'0000'0010'0000,
        0b0000'0101'0101'1111,
        {0b0000'0010'0000'0000},
        {0b0010'0000'0000'0000}
    );
}

TEST_CASE("Illegal passive move up - on edge")
{
    checkPassiveMoves<Direction::Up>(
        0b0010'0000'0000'0000,
        empty,
        {},
        {}
    );
}

TEST_CASE("Illegal passive move up - pushing enemy stone")
{
    checkPassiveMoves<Direction::Up>(
        0b0000'0000'0100'0000,
        0b0000'0100'0000'0000,
        {},
        {}
    );
}

TEST_CASE("Passive move up - one illegal pushing own stone, one fine")
{
    checkPassiveMoves<Direction::Up>(
        0b0000'0100'0100'0000,
        empty,
        {0b0100'0000'0100'0000},
        {}
    );
}

#pragma endregion


#pragma region Down

TEST_CASE("Passive move down - one stone")
{
    checkPassiveMoves<Direction::Down>(
        0b0000'0000'1000'0000,
        empty,
        {0b0000'0000'0000'1000},
        {}
    );
}

TEST_CASE("Illegal passive move down - on edge")
{
    checkPassiveMoves<Direction::Down>(
        0b0000'0000'0000'0010,
        empty,
        {},
        {}
    );
}

#pragma endregion


#pragma region Left

TEST_CASE("Passive move left - one stone")
{
    checkPassiveMoves<Direction::Left>(
        0b0000'0010'0000'0000,
        empty,
        {0b0000'0100'0000'0000},
        {0b0000'1000'0000'0000}
    );
}

TEST_CASE("Illegal passive move left - on edge")
{
    checkPassiveMoves<Direction::Left>(
        0b0000'0000'1000'0000,
        empty,
        {},
        {}
    );
}

#pragma endregion


#pragma region Right

TEST_CASE("Passive move right - one stone two moves")
{
    checkPassiveMoves<Direction::Right>(
        0b0000'0000'1000'0000,
        empty,
        {0b0000'0000'0100'0000},
        {0b0000'0000'0010'0000}
    );
}

TEST_CASE("Passive move right - one stone one move")
{
    checkPassiveMoves<Direction::Right>(
        0b0000'0000'0010'0000,
        empty,
        {0b0000'0000'0001'0000},
        {}
    );
}

TEST_CASE("Illegal passive move down - on edge")
{
    checkPassiveMoves<Direction::Right>(
        0b0000'0000'0001'0000,
        empty,
        {},
        {}
    );
}

#pragma endregion


#pragma region UpRight

TEST_CASE("Passive move up right - one stone two moves")
{
    checkPassiveMoves<Direction::UpRight>(
        0b0000'0000'1000'0000,
        empty,
        {0b0000'0100'0000'0000},
        {0b0010'0000'0000'0000}
    );
}

TEST_CASE("Passive move up right - one stone one move")
{
    checkPassiveMoves<Direction::UpRight>(
        0b0000'0000'0010'0000,
        empty,
        {0b0000'0001'0000'0000},
        {}
    );
}

TEST_CASE("Illegal passive move up right - on edge")
{
    checkPassiveMoves<Direction::UpRight>(
        0b0001'0000'0000'0000,
        empty,
        {},
        {}
    );
}

#pragma endregion


#pragma region DownRight

TEST_CASE("Passive move down right - one stone")
{
    checkPassiveMoves<Direction::DownRight>(
        0b0000'0010'0000'0000,
        empty,
        {0b0000'0000'0001'0000},
        {}
    );
}

TEST_CASE("Illegal passive move down right - on edge")
{
    checkPassiveMoves<Direction::DownRight>(
        0b0000'0000'0000'0001,
        empty,
        {},
        {}
    );
}

#pragma endregion


#pragma region DownLeft

TEST_CASE("Passive move down left - one stone")
{
    checkPassiveMoves<Direction::DownLeft>(
        0b0010'0000'0000'0000,
        empty,
        {0b0000'0100'0000'0000},
        {0b0000'0000'1000'0000}
    );
}

TEST_CASE("Illegal passive move down left - on edge")
{
    checkPassiveMoves<Direction::DownLeft>(
        0b0000'0000'0000'0001,
        empty,
        {},
        {}
    );
}

#pragma endregion


#pragma region UpLeft

TEST_CASE("Passive move up left - one stone")
{
    checkPassiveMoves<Direction::UpLeft>(
        0b0000'0000'0100'0000,
        empty,
        {0b0000'1000'0000'0000},
        {}
    );
}

TEST_CASE("Illegal passive move up left - on edge")
{
    checkPassiveMoves<Direction::UpLeft>(
        0b1000'0000'0000'0000,
        empty,
        {},
        {}
    );
}

#pragma endregion
