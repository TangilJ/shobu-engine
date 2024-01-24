#include "perft.h"

#include <chrono>
#include <cstdio>

#include "consts.h"
#include "movegen.h"
#include "search.h"
#include "types.h"

uint64_t perft(const State state, const int depth)
{
    if (depth == 0 || checkWin(state) != Win::GameOngoing)
        return 1;

    States states;
    addMoves(state, states);

    uint64_t total = 0;
    for (int i = 0; i < states.length(); ++i)
    {
        State reversedPlayers{};
        reversedPlayers.own = {
            state.enemy[3], state.enemy[2],
            state.enemy[1], state.enemy[0]
        };
        reversedPlayers.enemy = {
            state.own[3], state.own[2],
            state.own[1], state.own[0]
        };
        total += perft(reversedPlayers, depth - 1);
    }

    return total;
}

void timeIncreasingPerft(const int depth)
{
    for (int currentDepth = 0; currentDepth < depth + 1; ++currentDepth)
    {
        auto start = std::chrono::steady_clock::now();
        const auto total = perft(startState, currentDepth);
        auto end = std::chrono::steady_clock::now();

        auto clockDuration = std::chrono::duration_cast<std::chrono::duration
            <double>>(end - start).count();
        auto nodesPerSecond = static_cast<double>(total) / clockDuration /
            1'000'000;

        printf("Depth %i: %10llu nodes - %6.1fs - %6.1f Mnodes/s\n",
               currentDepth, total, clockDuration, nodesPerSecond);
    }
}
