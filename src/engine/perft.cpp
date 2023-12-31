#include "perft.h"

#include <chrono>
#include <cstdint>
#include <ctime>

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
        State reversedPlayers = state;
        reversedPlayers.own = state.enemy;
        reversedPlayers.enemy = state.own;
        total += perft(reversedPlayers, depth - 1);
    }

    return total;
}

void timeIncreasingPerft(const int depth)
{
    for (int currentDepth = 1; currentDepth < depth + 1; ++currentDepth)
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
