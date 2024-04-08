#include <iostream>
#include "types.h"
#include "consts.h"
#include "print.h"

template<Direction direction>
constexpr Bitboard move(const Bitboard board)
{
    // @formatter:off
    return direction == Direction::Up        ? board << 4
         : direction == Direction::UpRight   ? board << 3 & ~left
         : direction == Direction::Right     ? board >> 1 & ~left
         : direction == Direction::DownRight ? board >> 5 & ~left
         : direction == Direction::Down      ? board >> 4
         : direction == Direction::DownLeft  ? board >> 3 & ~right
         : direction == Direction::Left      ? board << 1 & ~right
         : direction == Direction::UpLeft    ? board << 5 & ~right
         : 0;
    // @formatter:on
}

// Is the target allowed to be where it is, given the current quarterboard?
bool isPassiveMoveLegal(Bitboard target,
                        Bitboard ownStones,
                        Bitboard enemyStones)
{
    // Passive move not allowed if it would push another stone
    const bool blocked = (ownStones | enemyStones) & target;

    // Passive move not allowed if it would push self off board.
    // move() returns 0 if pushing itself off so moveOne == 0 if on edge
    const bool onEdge = !target;

    return !(blocked || onEdge);
}

template<Direction direction>
bool isAggressiveMoveLegal(const Quarter quarter,
                           const Bitboard stone)
{
    // Aggressive move not allowed when pushing more than 1 stone
    const Bitboard moveOne = move<direction>(stone);
    const Bitboard moveTwo = move<direction>(moveOne);
    const Bitboard blockPath = moveOne | moveTwo;
    const bool stonesInBlockPath =
        moveTwo != empty
        && ((quarter.own | quarter.enemy) & blockPath) == blockPath;

    // Aggressive move not allowed when pushing own stone
    const bool pushingOwnStone = quarter.own & moveOne;

    // Aggressive move not allowed if it would push self off board
    // move() returns 0 if pushing itself off so moveOne == 0 if on edge
    const bool onEdge = !moveOne;

    return !(stonesInBlockPath || pushingOwnStone || onEdge);
}

template<Direction direction>
Quarter applyAggressiveMove(const Quarter quarter,
                            const Bitboard stone)
{
    const Bitboard moveOne = move<direction>(stone);

    // Move own stone up. Remove current stone and place it one square above
    const Bitboard ownStonesMoved = quarter.own ^ stone | moveOne;

    // Enemy stone is pushed up if it exists. If there is an enemy stone one
    // square in next square, duplicate it up and remove its previous position
    const Bitboard enemyStonesMoved = move<direction>(moveOne & quarter.enemy)
                                      | quarter.enemy & ~moveOne;

    return {ownStonesMoved, enemyStonesMoved};
}

template<Direction direction>
void generatePassiveMoves(const Quarter quarter,
                          Vec<Bitboard, 4> &moveOnes,
                          Vec<Bitboard, 4> &moveTwos)
{
    Bitboard stonesLeft = quarter.own;
    while (stonesLeft)
    {
        const Bitboard stone = stonesLeft & -stonesLeft;
        stonesLeft ^= stone;

        // First passive move
        const Bitboard moveOne = move<direction>(stone);
        const bool allowed = isPassiveMoveLegal(
            moveOne, quarter.own, quarter.enemy
        );
        if (!allowed)
            continue;

        Bitboard ownAfterPassive = quarter.own ^ stone | moveOne;
        moveOnes.add(ownAfterPassive);

        // Second passive move if first move was allowed
        const Bitboard moveTwo = move<direction>(moveOne);
        const bool allowed2 = isPassiveMoveLegal(
            moveTwo, ownAfterPassive, quarter.enemy
        );
        if (!allowed2)
            continue;

        Bitboard ownAfterPassive2 = ownAfterPassive ^ moveOne | moveTwo;
        moveTwos.add(ownAfterPassive2);
    }
}

template<Direction direction>
void generateAggressiveMoves(const Quarter quarter,
                             Vec<Quarter, 4> &moveOnes,
                             Vec<Quarter, 4> &moveTwos)
{
    Bitboard stonesLeft = quarter.own;
    while (stonesLeft)
    {
        const Bitboard stone = stonesLeft & -stonesLeft;
        stonesLeft ^= stone;

        // First aggressive move
        bool legal = isAggressiveMoveLegal<direction>(quarter, stone);
        if (!legal) continue;

        const Quarter aggr1 = applyAggressiveMove<direction>(quarter, stone);
        moveOnes.add(aggr1);


        // Second aggressive move if first move was allowed
        const Bitboard moveOne = move<direction>(stone);
        bool legal2 = isAggressiveMoveLegal<direction>(aggr1, moveOne);
        if (!legal2) continue;

        const Quarter aggr2 = applyAggressiveMove<direction>(
            aggr1,
            moveOne
        );
        moveTwos.add(aggr2);
    }
}


template<Location passiveBoard, Location aggressiveBoard>
void combinePassiveAggressive(const State &state, std::vector<State> &states,
                              Vec<Bitboard, 4> &passiveMoveOnes,
                              Vec<Quarter, 4> &aggressiveMoveOnes)
{
    for (const Quarter &aggr: aggressiveMoveOnes)
    {
        for (const Bitboard &passive: passiveMoveOnes)
        {
            Quarter newPassive = {
                passive,
                state.getQuarter<passiveBoard>().enemy
            };

            State newState = state.setQuarter<passiveBoard>(newPassive);
            newState = newState.setQuarter<aggressiveBoard>(aggr);
            states.push_back(newState);
        }
    }
}

template<Direction direction, Location passiveBoard, Location aggressiveBoard>
void generateMovesOnBoard(const State &state,
                          std::vector<State> &states)
{
    Vec<Bitboard, 4> passiveMoveOnes = {};
    Vec<Bitboard, 4> passiveMoveTwos = {};
    generatePassiveMoves<direction>(
        state.getQuarter<passiveBoard>(),
        passiveMoveOnes,
        passiveMoveTwos
    );

    Vec<Quarter, 4> aggressiveMoveOnes = {};
    Vec<Quarter, 4> aggressiveMoveTwos = {};
    generateAggressiveMoves<direction>(
        state.getQuarter<aggressiveBoard>(),
        aggressiveMoveOnes,
        aggressiveMoveTwos
    );

    combinePassiveAggressive<passiveBoard, aggressiveBoard>(
        state, states,
        passiveMoveOnes, aggressiveMoveOnes
    );
    combinePassiveAggressive<passiveBoard, aggressiveBoard>(
        state, states,
        passiveMoveTwos, aggressiveMoveTwos
    );
}

template<Direction direction>
void generateMovesForDirection(const State state, std::vector<State> &states)
{
    // Note: bottom boards are always homeboards
    generateMovesOnBoard<direction, Location::BottomLeft, Location::TopRight>(state, states);
    generateMovesOnBoard<direction, Location::BottomLeft, Location::BottomRight>(state, states);
    generateMovesOnBoard<direction, Location::BottomRight, Location::TopLeft>(state, states);
    generateMovesOnBoard<direction, Location::BottomRight, Location::BottomLeft>(state, states);
}

void generateAllMovesInPly(const State state, std::vector<State> &states)
{
    generateMovesForDirection<Direction::Up>(state, states);
    generateMovesForDirection<Direction::UpRight>(state, states);
    generateMovesForDirection<Direction::Right>(state, states);
    generateMovesForDirection<Direction::DownRight>(state, states);
    generateMovesForDirection<Direction::Down>(state, states);
    generateMovesForDirection<Direction::DownLeft>(state, states);
    generateMovesForDirection<Direction::Left>(state, states);
    generateMovesForDirection<Direction::UpLeft>(state, states);
}
