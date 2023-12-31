#include "types.h"
#include "print.h"

#include <iostream>
#include <bitset>
#include <cassert>

std::string toBitString(const Board board)
{
    const std::bitset<16> x(board);
    return x.to_string();
}

void printBits(const Board board)
{
    std::cout << toBitString(board) << std::endl;
}

void print(const Board board)
{
    Board cell = 0b1000'0000'0000'0000;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (cell & board)
                std::cout << 'o';
            else
                std::cout << '.';
            cell >>= 1;
        }
        std::cout << std::endl;
    }
}

// Own homeboards are bottom 2 boards, enemy homeboards are top 2 boards
void print(const State state)
{
    printTwoBoards(state.own[0], state.own[1],
                   state.enemy[0], state.enemy[1]);
    std::cout << std::endl;
    printTwoBoards(state.own[2], state.own[3],
                   state.enemy[2], state.enemy[3]);
    std::cout << std::endl;
}

void printTwoBoards(const TwoBoards boards)
{
    const Board first = boards >> 16;
    const Board second = static_cast<Board>(boards);
    printTwoBoards(first, second, 0, 0);
}

void printTwoBoards(const Board ownLeft, const Board ownRight,
                    const Board enemyLeft, const Board enemyRight)
{
    Board leftCell = 0b1000'0000'0000'0000;
    Board rightCell = 0b1000'0000'0000'0000;

    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            assert(!((leftCell & ownLeft) && (leftCell & enemyLeft)));
            if (leftCell & ownLeft)
                std::cout << 'o';
            else if (leftCell & enemyLeft)
                std::cout << 'x';
            else
                std::cout << '.';
            leftCell >>= 1;
        }

        std::cout << " | ";

        for (int j = 0; j < 4; ++j)
        {
            assert(!((rightCell & ownRight) && (rightCell & enemyRight)));
            if (rightCell & ownRight)
                std::cout << 'o';
            else if (rightCell & enemyRight)
                std::cout << 'x';
            else
                std::cout << '.';
            rightCell >>= 1;
        }

        std::cout << std::endl;
    }
}
