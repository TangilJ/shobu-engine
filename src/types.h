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

enum BoardType : int {
    TopLeft = 0,
    TopRight = 1,
    BottomLeft = 2,
    BottomRight = 3
};

// Index 0 = top left board, 1 = top right, 2 = bottom left, 3 = bottom right 
// TODO: Use std::array instead?
struct State {
    Board own[4];
    Board enemy[4];
};


struct States {
private:
    static constexpr int arrayMaxSize = 100;
    unsigned short size = 0;
    std::array<State, arrayMaxSize> array{};
public:
    State operator[](int i)
    {
        return array[i];
    }

    void add(State &state)
    {
        array[size] = state;
        size++;
    }
};

#endif //SHOBU_TYPES_H
