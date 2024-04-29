#ifndef NNUE_ACCUMULATOR_H
#define NNUE_ACCUMULATOR_H

#include <array>
#include "linear.h"

enum Player : int
{
    P1 = 0,
    P2 = 1
};

template<int size>
struct Accumulator
{
    std::array<float, size> p1;
    std::array<float, size> p2;

    template<Player player>
    float operator [](int idx)
    {
        return player == P1 ? p1[idx] : p2[idx];
    }

    template<int inputs>
    Accumulator(Linear<inputs, size> linear, std::vector<int> &activeIdxs)
    {
        for (int i = 0; i < size; ++i)
            p1[i] = linear.biases[i];

        for (const int a: activeIdxs)
            for (int i = 0; i < size; ++i)
                p1[i] += linear.weights[a][i];

        // Player 1 and 2's accumulated values are
        // the same at the start of the game
        p2 = p1;
    }

    template<int inputs, Player player>
    Accumulator<size> update(Linear<inputs, size> linear,
                             std::vector<int> &added,
                             std::vector<int> &removed)
    {
        Accumulator<size> updated = *this;

        for (const int a: added)
            for (int i = 0; i < size; i++)
                updated[player][i] += linear.weights[a][i];
        
        for (const int r: removed)
            for (int i = 0; i < size; i++)
                updated[player][i] -= linear.weights[r][i];
        
        return updated;
    }
};

#endif //NNUE_ACCUMULATOR_H
