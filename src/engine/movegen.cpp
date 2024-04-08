#include <iostream>
#include "types.h"
#include "consts.h"
#include "print.h"

template<Direction Direction>
constexpr Bitboard move(const Bitboard board)
{
    // @formatter:off
    return Direction == Direction::Up        ? board << 4
         : Direction == Direction::UpRight   ? board << 3 & ~left
         : Direction == Direction::Right     ? board >> 1 & ~left
         : Direction == Direction::DownRight ? board >> 5 & ~left
         : Direction == Direction::Down      ? board >> 4
         : Direction == Direction::DownLeft  ? board >> 3 & ~right
         : Direction == Direction::Left      ? board << 1 & ~right
         : Direction == Direction::UpLeft    ? board << 5 & ~right
         : 0;
    // @formatter:on
}

// Is the target allowed to be where it is, given the current quarterboard?
bool
passiveMoveAllowed(Bitboard target, Bitboard ownStones, Bitboard enemyStones)
{
    // Passive move not allowed if it would push another stone
    const bool blocked = (ownStones | enemyStones) & target;

    // Passive move not allowed if it would push self off board.
    // move() returns 0 if pushing itself off so moveOne == 0 if on edge
    const bool onEdge = !target;

    return !(blocked || onEdge);
}

template<Direction Direction>
bool isAggressiveMoveLegal(const Quarterboard quarter,
                           const Bitboard stone)
{
    // Aggressive move not allowed when pushing more than 1 stone
    const Bitboard moveOne = move<Direction>(stone);
    const Bitboard moveTwo = move<Direction>(moveOne);
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

template<Direction Direction>
Quarterboard applyAggressiveMove(const Quarterboard quarter,
                                 const Bitboard stone)
{
    const Bitboard moveOne = move<Direction>(stone);

    // Move own stone up. Remove current stone and place it one square above
    const Bitboard ownStonesMoved = quarter.own ^ stone | moveOne;

    // Enemy stone is pushed up if it exists. If there is an enemy stone one
    // square in next square, duplicate it up and remove its previous position
    const Bitboard enemyStonesMoved = move<Direction>(moveOne & quarter.enemy)
                                      | quarter.enemy & ~moveOne;

    return {ownStonesMoved, enemyStonesMoved};
}

template<Direction Direction>
void generatePassiveMoves(const Quarterboard quarter,
                          Vec<Bitboard, 4> &moveOnes,
                          Vec<Bitboard, 4> &moveTwos)
{
    Bitboard stonesLeft = quarter.own;
    while (stonesLeft)
    {
        const Bitboard stone = stonesLeft & -stonesLeft;
        stonesLeft ^= stone;

        // First passive move
        const Bitboard moveOne = move<Direction>(stone);
        const bool allowed = passiveMoveAllowed(
            moveOne, quarter.own, quarter.enemy
        );
        if (!allowed)
            continue;

        Bitboard ownAfterPassive = quarter.own ^ stone | moveOne;
        moveOnes.add(ownAfterPassive);

        // Second passive move if first move was allowed
        const Bitboard moveTwo = move<Direction>(moveOne);
        const bool allowed2 = passiveMoveAllowed(
            moveTwo, ownAfterPassive, quarter.enemy
        );
        if (!allowed2)
            continue;

        Bitboard ownAfterPassive2 = ownAfterPassive ^ moveOne | moveTwo;
        moveTwos.add(ownAfterPassive2);
    }
}

template<Direction Direction>
void generateAggressiveMoves(const Quarterboard quarter,
                             Vec<Quarterboard, 4> &moveOnes,
                             Vec<Quarterboard, 4> &moveTwos)
{
    Bitboard stonesLeft = quarter.own;
    while (stonesLeft)
    {
        const Bitboard stone = stonesLeft & -stonesLeft;
        stonesLeft ^= stone;

        // First aggressive move
        bool legal = isAggressiveMoveLegal<Direction>(quarter, stone);
        if (!legal) continue;

        const Quarterboard aggr1 = applyAggressiveMove<Direction>(quarter, stone);
        moveOnes.add(aggr1);


        // Second aggressive move if first move was allowed
        const Bitboard moveOne = move<Direction>(stone);
        bool legal2 = isAggressiveMoveLegal<Direction>(aggr1, moveOne);
        if (!legal2) continue;

        const Quarterboard aggr2 = applyAggressiveMove<Direction>(
            aggr1,
            moveOne
        );
        moveTwos.add(aggr2);
    }
}


template<Quarter passiveBoard, Quarter aggressiveBoard>
void combinePassiveAggressive(const State &state, std::vector<State> &states,
                              Vec<Bitboard, 4> &passiveMoveOnes,
                              Vec<Quarterboard, 4> &aggressiveMoveOnes)
{
    for (const Quarterboard &aggr: aggressiveMoveOnes)
    {
        for (const Bitboard &passive: passiveMoveOnes)
        {
            Quarterboard newPassive = {
                passive,
                state.getQuarter<passiveBoard>().enemy
            };

            State newState = state.setQuarter<passiveBoard>(newPassive);
            newState = newState.setQuarter<aggressiveBoard>(aggr);
            states.push_back(newState);
        }
    }
}

template<Direction Direction, Quarter passiveBoard, Quarter aggressiveBoard>
void generateMovesOnBoard(const State &state,
                          std::vector<State> &states)
{
    Vec<Bitboard, 4> passiveMoveOnes = {};
    Vec<Bitboard, 4> passiveMoveTwos = {};
    generatePassiveMoves<Direction>(
        state.getQuarter<passiveBoard>(),
        passiveMoveOnes,
        passiveMoveTwos
    );

    Vec<Quarterboard, 4> aggressiveMoveOnes = {};
    Vec<Quarterboard, 4> aggressiveMoveTwos = {};
    generateAggressiveMoves<Direction>(
        state.getQuarter<aggressiveBoard>(),
        aggressiveMoveOnes,
        aggressiveMoveTwos
    );

    combinePassiveAggressive<passiveBoard, aggressiveBoard>(state, states, passiveMoveOnes, aggressiveMoveOnes);
    combinePassiveAggressive<passiveBoard, aggressiveBoard>(state, states, passiveMoveTwos, aggressiveMoveTwos);
}

template<Direction Direction>
void generateMovesForDirection(const State state, std::vector<State> &states)
{
    // Note: bottom boards are always homeboards
    generateMovesOnBoard<Direction, Quarter::BottomLeft, Quarter::TopRight>(state, states);
    generateMovesOnBoard<Direction, Quarter::BottomLeft, Quarter::BottomRight>(state, states);
    generateMovesOnBoard<Direction, Quarter::BottomRight, Quarter::TopLeft>(state, states);
    generateMovesOnBoard<Direction, Quarter::BottomRight, Quarter::BottomLeft>(state, states);
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
