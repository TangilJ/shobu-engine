#include <doctest/doctest.h>
#include "movegen.h"
#include "print.h"

TEST_CASE("Aggressive move up - 1 own, 0 enemy")
{
    const Board own = 0b0000'0000'0100'0000;
    const Board enemy = 0b0000'0000'0000'0000;
    const Board stone = 0b0000'0000'0100'0000;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    const Board ownResult = result >> 16;
    const Board enemyResult = (Board) result;

    const Board ownExpected = 0b0000'0100'0000'0000;
    const Board enemyExpected = 0b0000'0000'0000'0000;

    CHECK(ownExpected == ownResult);
    CHECK(enemyExpected == enemyResult);
}

TEST_CASE("Aggressive move up - many own, 0 enemy")
{
    const Board own = 0b0110'1010'0100'0110;
    const Board enemy = 0b0000'0000'0000'0000;
    const Board stone = 0b0000'0000'0100'0000;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    const Board ownResult = result >> 16;
    const Board enemyResult = (Board) result;

    const Board ownExpected = 0b0110'1110'0000'0110;
    const Board enemyExpected = 0b0000'0000'0000'0000;

    CHECK(ownExpected == ownResult);
    CHECK(enemyExpected == enemyResult);
}

TEST_CASE("Aggressive move up - 1 own, pushing 1 enemy")
{
    const Board own = 0b0000'0000'0000'1000;
    const Board enemy = 0b0000'0000'1000'0000;
    const Board stone = 0b0000'0000'0000'1000;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    const Board ownResult = result >> 16;
    const Board enemyResult = (Board) result;

    const Board ownExpected = 0b0000'0000'1000'0000;
    const Board enemyExpected = 0b0000'1000'0000'0000;

    CHECK(ownExpected == ownResult);
    CHECK(enemyExpected == enemyResult);
}

TEST_CASE("Aggressive move up - many own, pushing 1 enemy of many")
{
    const Board own = 0b0110'1000'0100'0110;
    const Board enemy = 0b0000'0000'0010'1001;
    const Board stone = 0b0000'0000'0000'0010;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    const Board ownResult = result >> 16;
    const Board enemyResult = (Board) result;

    const Board ownExpected = 0b0110'1000'0110'0100;
    const Board enemyExpected = 0b0000'0010'0000'1001;

    CHECK(ownExpected == ownResult);

    CHECK(enemyExpected == enemyResult);
}

TEST_CASE("Illegal aggressive move up - on edge")
{
    const Board own = 0b0100'0000'0000'0000;
    const Board enemy = 0b0000'0000'0000'0000;
    const Board stone = 0b0100'0000'0000'0000;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    CHECK(0 == result);
}

TEST_CASE("Illegal aggressive move up - pushing 2 enemy stones")
{
    const Board own = 0b0000'0000'0010'0000;
    const Board enemy = 0b0010'0010'0000'0000;
    const Board stone = 0b0000'0000'0010'0000;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    CHECK(0 == result);
}

TEST_CASE("Illegal aggressive move up - pushing enemy stone into own")
{
    const Board own = 0b0010'0000'0010'0000;
    const Board enemy = 0b0000'0010'0000'0000;
    const Board stone = 0b0000'0000'0010'0000;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    CHECK(0 == result);
}


TEST_CASE("Illegal aggressive move up - pushing own stone")
{
    const Board own = 0b0000'0010'0010'0000;
    const Board enemy = 0b0000'0000'0000'0000;
    const Board stone = 0b0000'0000'0010'0000;

    const TwoBoards result = applyStoneAggressiveMove(own, enemy, stone);
    CHECK(0 == result);
}

