#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>
#include "movegen.h"
#include "types.h"


TEST_CASE("Passive move up - one stone")
{
    States states;
    const BoardArray own = {
        0b0000'0000'0000'1000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    const BoardArray enemy = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    State state = {own, enemy};

    addPassiveMoves<Direction::Up>(TopLeft, state, states);

    const BoardArray expectedOwn = {
        0b0000'0000'1000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    CHECK(1 == states.length());
    CHECK(expectedOwn == states[0].own);
}

TEST_CASE("Passive move up - four stones")
{
    States states;
    const BoardArray own = {
        0b0000'0001'0010'1100,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    const BoardArray enemy = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    State state = {own, enemy};

    addPassiveMoves<Direction::Up>(TopLeft, state, states);

    const BoardArray expectedOwn0 = {
        0b0000'0001'0110'1000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };

    const BoardArray expectedOwn1 = {
        0b0000'0001'1010'0100,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };

    const BoardArray expectedOwn2 = {
        0b0000'0011'0000'1100,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };

    const BoardArray expectedOwn3 = {
        0b0001'0000'0010'1100,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
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
    const BoardArray own = {
        0b0000'0000'0000'0000,
        0b0000'0000'0010'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    const BoardArray enemy = {
        0b0000'0000'0000'0000,
        0b0000'0101'0101'1111,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    State state = {own, enemy};

    addPassiveMoves<Direction::Up>(TopRight, state, states);

    const BoardArray expectedOwn = {
        0b0000'0000'0000'0000,
        0b0000'0010'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    CHECK(1 == states.length());
    CHECK(expectedOwn == states[0].own);
}


TEST_CASE("Illegal passive move up - on edge")
{
    States states;
    const BoardArray own = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0010'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    const BoardArray enemy = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    State state = {own, enemy};

    addPassiveMoves<Direction::Up>(BottomLeft, state, states);

    CHECK(0 == states.length());
}

TEST_CASE("Illegal passive move up - pushing enemy stone")
{
    States states;
    const BoardArray own = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0100'0000
    };
    const BoardArray enemy = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0100'0000'0000
    };
    State state = {own, enemy};

    addPassiveMoves<Direction::Up>(BottomRight, state, states);

    CHECK(0 == states.length());
}

TEST_CASE("Passive move up - one illegal pushing own stone, one fine")
{
    States states;
    const BoardArray own = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0100'0100'0000
    };
    const BoardArray enemy = {
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000,
        0b0000'0000'0000'0000
    };
    State state = {own, enemy};

    addPassiveMoves<Direction::Up>(BottomRight, state, states);

    CHECK(1 == states.length());
}
