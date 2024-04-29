#ifndef NNUE_ACCUMULATOR_H
#define NNUE_ACCUMULATOR_H

#include <array>
#include <set>
#include <future>
#include "linear.h"
#include "src/engine/types.h"
#include "src/engine/consts.h"
#include "src/engine/movegen.h"

enum Player : int
{
    P1 = 0,
    P2 = 1
};

template<int size>
class Accumulator
{
public:
    std::array<float, size> p1;
    std::array<float, size> p2;
    Board p1Board{};
    Board p2Board{};

    template<int inputs>
    Accumulator(Linear<inputs, size> linear, Board board)
    {
        for (int i = 0; i < size; ++i)
            p1[i] = linear.biases[i];

        p2 = p1; // Player 1 and 2's biases are the same

        for (const int a: getActiveIndexes(board))
            for (int i = 0; i < size; ++i)
                p1[i] += linear.weights[a][i];

        for (const int a: getActiveIndexes(reverseBoard(board)))
            for (int i = 0; i < size; ++i)
                p2[i] += linear.weights[a][i];

        p1Board = startState.board;
        p2Board = startState.board;
    }

    template<int inputs>
    Accumulator<size> update(Linear<inputs, size> linear, Board current, Player player)
    {
        std::vector<int> added;
        std::vector<int> removed;

        auto activeIdxs = getActiveIndexes(current);
        auto prevActiveIdxs = getActiveIndexes(player == P1 ? p1Board : p2Board);

        std::set_difference(activeIdxs.begin(), activeIdxs.end(),
                            prevActiveIdxs.begin(), prevActiveIdxs.end(),
                            std::inserter(added, added.begin()));

        std::set_difference(prevActiveIdxs.begin(), prevActiveIdxs.end(),
                            activeIdxs.begin(), activeIdxs.end(),
                            std::inserter(removed, removed.begin()));

        auto updated = *this;
        updateFeatures<inputs>(updated, linear, added, removed, player);
        if (player == P1) updated.p1Board = current;
        else updated.p2Board = current;

        return updated;
    }

    std::array<float, size> &operator [](Player player)
    {
        return player == P1 ? p1 : p2;
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

private:
    template<int inputs>
    void updateFeatures(Accumulator<size> &updated,
                        Linear<inputs, size> linear,
                        std::vector<int> &added,
                        std::vector<int> &removed,
                        Player player)
    {
        for (const int a: added)
            for (int i = 0; i < size; i++)
                updated[player][i] += linear.weights[a][i];

        for (const int r: removed)
            for (int i = 0; i < size; i++)
                updated[player][i] -= linear.weights[r][i];

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
