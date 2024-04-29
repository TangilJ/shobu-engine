#ifndef NNUE_ACCUMULATOR_H
#define NNUE_ACCUMULATOR_H

#include <array>
#include <set>
#include <future>
#include "linear.h"
#include "src/engine/types.h"

enum Player : int
{
    P1 = 0,
    P2 = 1
};

template<int size>
class Accumulator
{
public:
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
                             Board previous,
                             Board current)
    {
        std::vector<int> added;
        std::vector<int> removed;

        auto activeIdxs = getActiveIndexes(current);
        auto prevActiveIdxs = getActiveIndexes(previous);

        std::set_difference(activeIdxs.begin(), activeIdxs.end(),
                            prevActiveIdxs.begin(), prevActiveIdxs.end(),
                            std::inserter(added, added.begin()));

        std::set_difference(prevActiveIdxs.begin(), prevActiveIdxs.end(),
                            activeIdxs.begin(), activeIdxs.end(),
                            std::inserter(removed, removed.begin()));

        return updateFeatures<player>(linear, added, removed);
    }

private:
    std::array<float, size> p1;
    std::array<float, size> p2;

    template<Player player>
    float operator [](int idx)
    {
        return player == P1 ? p1[idx] : p2[idx];
    }

    template<int inputs, Player player>
    Accumulator<size> updateFeatures(Linear<inputs, size> linear,
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

    static std::set<int> getActiveIndexes(Board board)
    {
        std::set<int> active;
        addQuarterIndexes<Location::TopLeft, false>(board, active);
        addQuarterIndexes<Location::TopRight, false>(board, active);
        addQuarterIndexes<Location::BottomLeft, false>(board, active);
        addQuarterIndexes<Location::BottomRight, false>(board, active);
        addQuarterIndexes<Location::TopLeft, true>(board, active);
        addQuarterIndexes<Location::TopRight, true>(board, active);
        addQuarterIndexes<Location::BottomLeft, true>(board, active);
        addQuarterIndexes<Location::BottomRight, true>(board, active);
        return active;
    }

    template<Location location, bool enemyPerspective>
    static void addQuarterIndexes(Board board, std::set<int> &active)
    {
        Bitboard own = board.getQuarter<location>().own;
        Bitboard enemy = board.getQuarter<location>().enemy;
        if (enemyPerspective)
            std::swap(own, enemy);

        int locationOffset = 16 * 4 * 2;
        int stonesLeft = _mm_popcnt_u32(own);
        int offset =
            (enemyPerspective ? 512 : 0)
            + (location == Location::TopRight ? locationOffset * 1 : 0)
            + (location == Location::BottomLeft ? locationOffset * 2 : 0)
            + (location == Location::BottomRight ? locationOffset * 3 : 0)
            + (stonesLeft - 1) * 16;
        int enemyOffset = 16 * 4;

        addActiveIndexes(own, active, offset);
        addActiveIndexes(enemy, active, offset + enemyOffset);
    }

    static void addActiveIndexes(Bitboard bitboard, std::set<int> &active, int offset)
    {
        Bitboard b = 0b1000'0000'0000'0000;
        int i = 0;
        while (b)
        {
            if (b & bitboard)
                active.insert(i + offset);
            b >>= 1;
            i++;
        }
    }
};

#endif //NNUE_ACCUMULATOR_H
