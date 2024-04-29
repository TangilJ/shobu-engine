#ifndef SHOBU_SEARCH_H
#define SHOBU_SEARCH_H

#include "src/engine/types.h"
#include "src/engine/movegen.h"
#include "accumulator.h"
#include "network.h"

struct Search
{
    Board board;
    float value;
};

// Negamax with alpha-beta pruning. color is -1 or 1.
Search negamaxWithAbPruning(State state, int depth, int colour,
                            float alpha, float beta,
                            Accumulator<512> accumulator,
                            Network<512, 64, 64> nnue, bool root = false)
{
    if (depth == 0 || state.win != Win::GameOngoing)
    {
        auto newAcc = accumulator.update<1024>(
            nnue.layer1, state.board, colour == 1 ? Player::P1 : Player::P2
        );
        float evaluation = nnue.forwardAfterInitial(colour == 1 ? newAcc.p1 : newAcc.p2);
        return {state.board, evaluation};
    }

    std::vector<State> states;
    states.reserve(250);
    generateAllMovesInPly(state.board, states);

    float bestValue = -INFINITY;
    Board bestState{};

    for (const auto &s: states)
    {
        Search search = negamaxWithAbPruning(
            s, depth - 1, colour * -1,
            -beta, -alpha,
            accumulator, nnue
        );

        if (bestValue < -search.value)
        {
            bestValue = -search.value;
            bestState = search.board;
        }

        alpha = std::max(alpha, bestValue);

        if (alpha >= beta)
            break;
    }

    if (!root)
        bestState = state.board;

    return {bestState, bestValue};
}

#endif //SHOBU_SEARCH_H
