#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "movegen.h"
#include "types.h"
#include "consts.h"

constexpr BoardArray emptyArray = {empty, empty, empty, empty};

template <Direction Direction>
void checkIllegal(const BoardType board, const BoardArray own,
                  const BoardArray enemy, const int legalStates)
{
    States states;
    const State state = {own, enemy};

    addPassiveMoves<Direction>(board, state, states);
    CHECK(legalStates == states.length());
}

#pragma region Up
TEST_CASE("Passive move up - one stone")
{
    States states;
    constexpr BoardArray own = {0b0000'0000'0000'1000, empty, empty, empty};

    constexpr State state = {own, emptyArray};

    addPassiveMoves<Direction::Up>(TopLeft, state, states);

    constexpr BoardArray expectedOwn = {
        0b0000'0000'1000'0000, empty, empty, empty
    };
    CHECK(1 == states.length());
    CHECK(expectedOwn == states[0].own);
}

TEST_CASE("Passive move up - four stones")
{
    States states;
    constexpr BoardArray own = {0b0000'0001'0010'1100, empty, empty, empty};

    State state = {own, emptyArray};

    addPassiveMoves<Direction::Up>(TopLeft, state, states);

    constexpr BoardArray expectedOwn0 = {
        0b0000'0001'0110'1000, empty, empty, empty
    };

    constexpr BoardArray expectedOwn1 = {
        0b0000'0001'1010'0100, empty, empty, empty
    };

    constexpr BoardArray expectedOwn2 = {
        0b0000'0011'0000'1100, empty, empty, empty
    };

    constexpr BoardArray expectedOwn3 = {
        0b0001'0000'0010'1100, empty, empty, empty
    };

    CHECK(4 == states.length());
    CHECK(expectedOwn0 == states[0].own);
    CHECK(expectedOwn1 == states[1].own);
    CHECK(expectedOwn2 == states[2].own);
    CHECK(expectedOwn3 == states[3].own);
}

TEST_CASE("Passive move up - surrounded by enemy stones")
{
    States states;
    constexpr BoardArray own = {empty, 0b0000'0000'0010'0000, empty, empty};
    constexpr BoardArray enemy = {empty, 0b0000'0101'0101'1111, empty, empty};
    constexpr State state = {own, enemy};

    addPassiveMoves<Direction::Up>(TopRight, state, states);

    constexpr BoardArray expectedOwn = {
        empty, 0b0000'0010'0000'0000, empty, empty
    };
    CHECK(1 == states.length());
    CHECK(expectedOwn == states[0].own);
}

TEST_CASE("Illegal passive move up - on edge")
{
    constexpr BoardArray own = {empty, empty, 0b0010'0000'0000'0000, empty};
    checkIllegal<Direction::Up>(BottomLeft, own, emptyArray, 0);
}

TEST_CASE("Illegal passive move up - pushing enemy stone")
{
    constexpr BoardArray own = {empty, empty, empty, 0b0000'0000'0100'0000};
    constexpr BoardArray enemy = {empty, empty, empty, 0b0000'0100'0000'0000};
    checkIllegal<Direction::Up>(BottomRight, own, enemy, 0);
}

TEST_CASE("Passive move up - one illegal pushing own stone, one fine")
{
    constexpr BoardArray own = {empty, empty, empty, 0b0000'0100'0100'0000};
    checkIllegal<Direction::Up>(BottomRight, own, emptyArray, 1);
}
#pragma endregion


#pragma region Down
TEST_CASE("Passive move down - one stone")
{
    constexpr BoardArray own = {0b0000'0000'1000'0000, empty, empty, empty};
    constexpr BoardArray expectedOwn = {
        0b0000'0000'0000'1000, empty, empty, empty
    };
    checkLegalOneMove<Direction::Down>(TopLeft, own, emptyArray, expectedOwn);
}

TEST_CASE("Illegal passive move down - on edge")
{
    constexpr BoardArray own = {empty, empty, 0b0010'0000'0000'0000, empty};
    checkIllegal<Direction::Up>(BottomRight, own, emptyArray, 0);
}
#pragma endregion
