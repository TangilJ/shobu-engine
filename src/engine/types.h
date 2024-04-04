#ifndef SHOBU_TYPES_H
#define SHOBU_TYPES_H

#include <cstdint>
#include <array>
#include <cassert>
#include <vector>

// 4x4 board
// Rightmost bit is top left of board
// E.g. 1110 0001 0000 0010 would represent:
// ooo.
// ...o
// ....
// ..o.
using Bitboard = uint16_t;

struct Quarterboard
{
    Bitboard own;
    Bitboard enemy;

    static Quarterboard empty()
    {
        return {0, 0};
    }

    bool isEmpty() const
    {
        return own == 0;
    }
};

enum BoardType
{
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3
};

enum class Direction
{
    Up,
    UpRight,
    Right,
    DownRight,
    Down,
    DownLeft,
    Left,
    UpLeft
};

using BoardArray = std::array<Bitboard, 4>;

// Index 0 = top left,    1 = top right,
//       2 = bottom left, 3 = bottom right
struct State
{
    BoardArray own;
    BoardArray enemy;
};

template<typename T, size_t n>
struct Vec
{
private:
    unsigned short size = 0;
    std::array<T, n> array{};

public:
    T operator[](const int i) const
    {
        return array[i];
    }

    auto add(const T &x)
    {
        assert(size <= n);
        array[size] = x;
        size++;
    }

    auto begin()
    {
        return array.begin();
    }

    auto end()
    {
        return array.begin() + size;
    }
};


#endif //SHOBU_TYPES_H
