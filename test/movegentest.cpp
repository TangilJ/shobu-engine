#include <doctest/doctest.h>
#include <algorithm>
#include "../src/engine/movegen.h"
#include "../src/engine/consts.h"

static void checkContains(std::vector<State> &states, Board state)
{
    CHECK(
        (std::ranges::any_of(
            states,
            [state](const State &s) { return s.board == state; }
        ))
    );
}

TEST_CASE("generateMovesForDirection - up two moves")
{
    Board state = {
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000}
    };

    std::vector<State> states;
    generateMovesForDirection<Direction::Up>(state, states);

    CHECK((states.size() == 8));
}

TEST_CASE("generateMovesForDirection - down left pushing")
{
    Board state = {
        {0b0000'0000'1000'0000, 0b1000'0000'0000'0000},
        {0b0000'0000'0100'0000, 0b0000'0000'0000'1000},
        {0b0000'0010'0000'0000, 0b1000'0000'0000'0000},
        {0b0000'0010'0000'0000, 0b0000'0000'0100'0000},
    };

    std::vector<State> states;
    generateMovesForDirection<Direction::DownLeft>(state, states);

    CHECK((states.size() == 3));
    checkContains(states, {
        {0b0000'0000'1000'0000, 0b1000'0000'0000'0000},
        {0b0000'0000'0100'0000, 0b0000'0000'0000'1000},
        {0b0000'0000'0100'0000, 0b1000'0000'0000'0000},
        {0b0000'0000'0100'0000, 0b0000'0000'0000'1000},
    });
    checkContains(states, {
        {0b0000'0000'1000'0000, 0b1000'0000'0000'0000},
        {0b0000'0000'0000'1000, empty},
        {0b0000'0000'0100'0000, 0b1000'0000'0000'0000},
        {0b0000'0010'0000'0000, 0b0000'0000'0100'0000},
    });
    checkContains(states, {
        {0b0000'0000'1000'0000, 0b1000'0000'0000'0000},
        {0b0000'0000'0100'0000, 0b0000'0000'0000'1000},
        {0b0000'0000'0000'1000, 0b1000'0000'0000'0000},
        {0b0000'0000'0000'1000, empty},
    });
}

TEST_CASE("generateAllMovesInPly")
{
    Board state = {
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0001, 0b0100'0000'0000'0000}
    };

    std::vector<State> states;
    generateAllMovesInPly(state, states);

    CHECK((states.size() == 26));
}

TEST_CASE("reverseBoard")
{
    Board board = {
        {0b0000010000001101, 0b1100000000000000},
        {0b0000000010100110, 0b1110000000000000},
        {0b0000000000101011, 0b1011000000000000},
        {0b0000000000011101, 0b1110000000000000},
    };
    Board reversed = reverseBoard(board);

    Board expected = {
        {0b0000000000000111, 0b1011100000000000},
        {0b0000000000001101, 0b1101010000000000},
        {0b0000000000000111, 0b0110010100000000},
        {0b0000000000000011, 0b1011000000100000},
    };

    CHECK((reversed == expected));
}