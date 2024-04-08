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

    bool operator==(const Quarterboard &rhs) const
    {
        return own == rhs.own &&
               enemy == rhs.enemy;
    }
};

enum class Quarter
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

// TODO: Pack into a uint64_t instead
using BoardArray = std::array<Bitboard, 4>;

struct State
{
    Quarterboard topLeft;
    Quarterboard topRight;
    Quarterboard bottomLeft;
    Quarterboard bottomRight;

    template<Quarter quarter>
    constexpr Quarterboard getQuarter() const
    {
        // @formatter:off
        return quarter == Quarter::TopLeft     ? topLeft
             : quarter == Quarter::TopRight    ? topRight
             : quarter == Quarter::BottomLeft  ? bottomLeft
             : quarter == Quarter::BottomRight ? bottomRight
             : throw;
        // @formatter:on
    }

    template<Quarter quarter>
    constexpr State setQuarter(const Quarterboard quarterboard) const
    {
        // @formatter:off
        return State{
            quarter == Quarter::TopLeft     ? quarterboard : topLeft,
            quarter == Quarter::TopRight    ? quarterboard : topRight,
            quarter == Quarter::BottomLeft  ? quarterboard : bottomLeft,
            quarter == Quarter::BottomRight ? quarterboard : bottomRight,
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
