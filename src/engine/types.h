#ifndef SHOBU_TYPES_H
#define SHOBU_TYPES_H

#include <cstdint>
#include <array>

// 4x4 board
// Rightmost bit is top left of board
// E.g. 1110 0001 0000 0010 would represent:
// ooo.
// ...o
// ....
// ..o.
using Board = uint16_t;

// TODO: Could use a wrapper if there is no performance degradation
using TwoBoards = uint32_t;

enum BoardType : int
{
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3
};

using BoardArray = std::array<Board, 4>;

// Index 0 = top left board, 1 = top right, 2 = bottom left, 3 = bottom right 
struct State
{
    BoardArray own;
    BoardArray enemy;
};


struct States
{
private:
    static constexpr int arrayMaxSize = 100;
    unsigned short size = 0;
    std::array<State, arrayMaxSize> array{};

public:
    [[nodiscard]] State operator[](const int i) const
    {
        return array[i];
    }

    void add(State& state)
    {
        array[size] = state;
        size++;
    }

    [[nodiscard]] int length() const
    {
        return size;
    }
};

#endif //SHOBU_TYPES_H
