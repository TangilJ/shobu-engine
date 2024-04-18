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

enum class Location : int
{
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3
};

enum class Direction : int
{
    Up = 0,
    UpRight = 1,
    Right = 2,
    DownRight = 3,
    Down = 4,
    DownLeft = 5,
    Left = 6,
    UpLeft = 7
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
            location == Location::TopLeft         ? quarterboard : topLeft,
            location == Location::TopRight       ? quarterboard : topRight,
            location == Location::BottomLeft   ? quarterboard : bottomLeft,
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
    std::array<int, MAX_SIZE> sourceIdxs{};

public:
    T operator [](const int i) const
    {
        return array[i];
    }

    Bitboard getSourceIndex(const int i) const
    {
        return sourceIdxs[i];
    }

    auto add(const T &x, const int &s)
    {
        assert(len <= MAX_SIZE);
        array[len] = x;
        sourceIdxs[len] = s;
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

enum class PassiveSide : int
{
    Left = 0,
    Right = 1
};

enum class AggressiveSide : int
{
    Top = 0,
    Bottom = 1
};

struct Move
{
    int passiveSourceIndex; // E.g. 0b0001'0000'0000'0000 -> 3
    int aggressiveSourceIndex;
    PassiveSide passiveSide;
    AggressiveSide aggressiveSide;
    Direction direction;
    int timesMoved;
};

enum class Win
{
    OwnWin,
    OpponentWin,
    GameOngoing
};

struct State
{
    Board board;
    Move move; // Move that led to this state
};

#endif //SHOBU_TYPES_H
