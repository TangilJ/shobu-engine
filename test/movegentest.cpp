#include <doctest/doctest.h>
#include "../src/engine/movegen.h"
#include "../src/engine/consts.h"

static void checkContains(std::vector<Board> &states, Board state)
{
    CHECK((std::find(states.begin(), states.end(), state) != states.end()));
}

TEST_CASE("generateMovesForDirection - up two moves")
{
    Board state = {
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000},
        {0b0000'0000'0000'0010, 0b0100'0000'0000'0000}
    };

    std::vector<Board> states;
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

    std::vector<Board> states;
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

    std::vector<Board> states;
    generateAllMovesInPly(state, states);

    CHECK((states.size() == 26));
}