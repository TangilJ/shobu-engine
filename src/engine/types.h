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

struct Quarter
{
    Bitboard own;
    Bitboard enemy;

    bool operator==(const Quarter &rhs) const
    {
        return own == rhs.own &&
               enemy == rhs.enemy;
    }
};

enum class Location
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
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

struct State
{
    Quarter topLeft;
    Quarter topRight;
    Quarter bottomLeft;
    Quarter bottomRight;

    template<Location location>
    constexpr Quarter getQuarter() const
    {
        // @formatter:off
        return location == Location::TopLeft     ? topLeft
             : location == Location::TopRight    ? topRight
             : location == Location::BottomLeft  ? bottomLeft
             : location == Location::BottomRight ? bottomRight
             : throw;
        // @formatter:on
    }

    template<Location location>
    constexpr State setQuarter(const Quarter quarterboard) const
    {
        // @formatter:off
        return State{
            location == Location::TopLeft     ? quarterboard : topLeft,
            location == Location::TopRight    ? quarterboard : topRight,
            location == Location::BottomLeft  ? quarterboard : bottomLeft,
            location == Location::BottomRight ? quarterboard : bottomRight,
        };
        // @formatter:on

    }

    bool operator==(const State &rhs) const
    {
        return topLeft == rhs.topLeft &&
               topRight == rhs.topRight &&
               bottomLeft == rhs.bottomLeft &&
               bottomRight == rhs.bottomRight;
    }
};

template<typename T, size_t maxSize>
struct Vec
{
private:
    unsigned short len = 0;
    std::array<T, maxSize> array{};

public:
    T operator[](const int i) const
    {
        return array[i];
    }

    auto add(const T &x)
    {
        assert(len <= maxSize);
        array[len] = x;
        len++;
    }

    auto begin()
    {
        return array.begin();
    }

    auto end()
    {
        return array.begin() + len;
    }

    auto size()
    {
        return len;
    }
};


#endif //SHOBU_TYPES_H
