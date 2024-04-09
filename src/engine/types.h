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

    bool operator ==(const Quarter &rhs) const
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

struct Board
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
    constexpr Board setQuarter(const Quarter quarterboard) const
    {
        // @formatter:off
        return Board{
            location == Location::TopLeft     ? quarterboard : topLeft,
            location == Location::TopRight    ? quarterboard : topRight,
            location == Location::BottomLeft  ? quarterboard : bottomLeft,
            location == Location::BottomRight ? quarterboard : bottomRight,
        };
        // @formatter:on

    }

    bool operator ==(const Board &rhs) const
    {
        return topLeft == rhs.topLeft &&
               topRight == rhs.topRight &&
               bottomLeft == rhs.bottomLeft &&
               bottomRight == rhs.bottomRight;
    }
};

template<typename T>
struct Moves
{
private:
    unsigned short len = 0;
    static constexpr int MAX_SIZE = 4;
    // Cannot have more than 4 stones, so max 4 moves
    std::array<T, MAX_SIZE> array{};
    std::array<Bitboard, MAX_SIZE> source{};

public:
    T operator [](const int i) const
    {
        return array[i];
    }

    Bitboard getSource(const int i) const
    {
        return source[i];
    }

    auto add(const T &x, const Bitboard &s)
    {
        assert(len <= MAX_SIZE);
        array[len] = x;
        source[len] = s;
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

struct Move
{
    Bitboard passiveSource;
    Bitboard aggressiveSource;
    Direction direction;
    int times;
};

struct State
{
    Board board;
    Move move; // Move that led to this state
};

#endif //SHOBU_TYPES_H
