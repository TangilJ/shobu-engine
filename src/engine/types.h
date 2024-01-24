#ifndef SHOBU_TYPES_H
#define SHOBU_TYPES_H

#include <cstdint>
#include <array>
#include <cassert>

// 4x4 board
// Rightmost bit is top left of board
// E.g. 1110 0001 0000 0010 would represent:
// ooo.
// ...o
// ....
// ..o.
using Board = uint16_t;

struct BoardPair {
    Board own;
    Board enemy;

    static BoardPair empty()
    {
        return {0, 0};
    }

    bool isEmpty() const
    {
        return own == 0;
    }
};

enum BoardType : int {
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3
};

enum class Direction {
    Up,
    UpRight,
    Right,
    DownRight,
    Down,
    DownLeft,
    Left,
    UpLeft
};

using BoardArray = std::array<Board, 4>;

// Index 0 = top left,    1 = top right,
//       2 = bottom left, 3 = bottom right 
// TODO: Use BoardPair for this
struct State {
    BoardArray own;
    BoardArray enemy;
};


struct States {
private:
    static constexpr int arrayMaxSize = 300;
    unsigned short size = 0;
    std::array<State, arrayMaxSize> array{};

public:
    [[nodiscard]] State operator[](const int i) const
    {
        return array[i];
    }

    void add(State &state)
    {
        assert(size <= arrayMaxSize);
        array[size] = state;
        size++;
    }

    [[nodiscard]] int length() const
    {
        return size;
    }
};

#endif //SHOBU_TYPES_H
