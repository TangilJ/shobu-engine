#include "types.h"
#include "print.h"

#include <iostream>
#include <bitset>
#include <cassert>

std::string toBitString(const Bitboard board)
{
    const std::bitset<16> x(board);
    return x.to_string();
}

void printBits(const Bitboard board)
{
    std::cout << toBitString(board) << std::endl;
}

void print(const Bitboard board)
{
    Bitboard cell = 0b1000'0000'0000'0000;
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
    std::cout << std::endl;
}

// Own homeboards are bottom 2 boards, enemy homeboards are top 2 boards
void print(const State state)
{
    printTwoBoards(state.topLeft.own, state.topRight.own,
                   state.topLeft.enemy, state.topRight.enemy);
    std::cout << std::endl;
    printTwoBoards(state.bottomLeft.own, state.bottomRight.own,
                   state.bottomLeft.enemy, state.bottomRight.enemy);
    std::cout << std::endl;
}

void printTwoBoards(const Quarter boards)
{
    printTwoBoards(boards.own, boards.enemy, 0, 0);
}

void printTwoBoards(const Bitboard ownLeft, const Bitboard ownRight,
                    const Bitboard enemyLeft, const Bitboard enemyRight)
{
    Bitboard leftCell = 0b1000'0000'0000'0000;
    Bitboard rightCell = 0b1000'0000'0000'0000;

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
