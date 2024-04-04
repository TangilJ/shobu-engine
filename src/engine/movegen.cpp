#include <iostream>
#include "types.h"
#include "consts.h"
#include "print.h"

template<Direction Direction>
constexpr Board move(const Board board)
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
bool passiveMoveAllowed(Board target, Board ownStones, Board enemyStones)
{
    // Passive move not allowed if it would push another stone
    const bool blocked = (ownStones | enemyStones) & target;

    // Passive move not allowed if it would push self off board.
    // move() returns 0 if pushing itself off so moveOne == 0 if on edge
    const bool onEdge = !target;

    return !(blocked || onEdge);
}

// Returns 0 if aggressive move not allowed
template<Direction Direction>
BoardPair aggressiveMove(const Board ownStones,
                         const Board enemyStones,
                         const Board stone)
{
    // TODO: Split into 2 smaller functions

#pragma region Move restrictions
    // Aggressive move not allowed when pushing more than 1 stone
    const Board moveOne = move<Direction>(stone);
    const Board moveTwo = move<Direction>(moveOne);
    const Board blockPath = moveOne | moveTwo;
    const bool stonesInBlockPath =
            moveTwo != empty
            && ((ownStones | enemyStones) & blockPath) == blockPath;

    // Aggressive move not allowed when pushing own stone
    const bool pushingOwnStone = ownStones & moveOne;

    // Aggressive move not allowed if it would push self off board
    // move() returns 0 if pushing itself off so moveOne == 0 if on edge
    const bool onEdge = !moveOne;

    if (stonesInBlockPath || pushingOwnStone || onEdge)
        return BoardPair::empty();
#pragma endregion

#pragma region Apply aggressive move
    // Move own stone up. Remove current stone and place it one square above
    const Board ownStonesMoved = ownStones ^ stone | moveOne;

    // Enemy stone is pushed up if it exists. If there is an enemy stone one
    // square in next square, duplicate it up and remove its previous position
    const Board enemyStonesMoved = move<Direction>(moveOne & enemyStones)
                                   | enemyStones & ~moveOne;
#pragma endregion

    return {ownStonesMoved, enemyStonesMoved};
}

template<Direction Direction>
void generatePassiveMoves(const Board own,
                          const Board enemy,
                          Vec<Board, 4> &moveOnes,
                          Vec<Board, 4> &moveTwos)
{
    Board stonesLeft = own;
    while (stonesLeft)
    {
        const Board stone = stonesLeft & -stonesLeft;
        stonesLeft ^= stone;

        // First passive move
        const Board moveOne = move<Direction>(stone);
        const bool allowed = passiveMoveAllowed(
                moveOne, own, enemy
        );
        if (!allowed)
            continue;

        Board ownAfterPassive = own ^ stone | moveOne;
        moveOnes.add(ownAfterPassive);

        // Second passive move if first move was allowed
        const Board moveTwo = move<Direction>(moveOne);
        const bool allowed2 = passiveMoveAllowed(
                moveTwo, ownAfterPassive, enemy
        );
        if (!allowed2)
            continue;

        Board ownAfterPassive2 = ownAfterPassive ^ moveOne | moveTwo;
        moveTwos.add(ownAfterPassive2);
    }
}

template<Direction Direction>
void generateAggressiveMoves(const Board own,
                             const Board enemy,
                             Vec<BoardPair, 4> &moveOnes,
                             Vec<BoardPair, 4> &moveTwos)
{
    Board stonesLeft = own;
    while (stonesLeft)
    {
        const Board stone = stonesLeft & -stonesLeft;
        stonesLeft ^= stone;

        // First aggressive move
        const BoardPair aggr1 = aggressiveMove<Direction>(own, enemy, stone);
        if (aggr1.isEmpty())
            continue;
        moveOnes.add(aggr1);

        // Second aggressive move if first move was allowed
        const Board moveOne = move<Direction>(stone);
        const BoardPair aggr2 = aggressiveMove<Direction>(aggr1.own,
                                                          aggr1.enemy,
                                                          moveOne);
        if (aggr2.isEmpty())
            continue;
        moveTwos.add(aggr2);
    }
}


template<Direction Direction>
void generateMovesOnBoard(const BoardType passiveBoard,
                          const BoardType aggressiveBoard,
                          const State &state,
                          std::vector<State> &states)
{
    // TODO: Profile if this is slower than pre-allocating with std::array

    Vec<Board, 4> passiveMoveOnes = {};
    Vec<Board, 4> passiveMoveTwos = {};
    generatePassiveMoves<Direction>(
            state.own[passiveBoard],
            state.enemy[passiveBoard],
            passiveMoveOnes,
            passiveMoveTwos
    );

    Vec<BoardPair, 4> aggressiveMoveOnes = {};
    Vec<BoardPair, 4> aggressiveMoveTwos = {};
    generateAggressiveMoves<Direction>(
            state.own[aggressiveBoard],
            state.enemy[aggressiveBoard],
            aggressiveMoveOnes,
            aggressiveMoveTwos
    );

    for (const BoardPair &aggr: aggressiveMoveOnes)
    {
        for (const Board &passive: passiveMoveOnes)
        {
            State newState = state;
            newState.own[passiveBoard] = passive;
            newState.own[aggressiveBoard] = aggr.own;
            newState.enemy[aggressiveBoard] = aggr.enemy;
            states.push_back(newState);
        }
    }

    for (const BoardPair &aggr: aggressiveMoveTwos)
    {
        for (const Board &passive: passiveMoveTwos)
        {
            State newState = state;
            newState.own[passiveBoard] = passive;
            newState.own[aggressiveBoard] = aggr.own;
            newState.enemy[aggressiveBoard] = aggr.enemy;
            states.push_back(newState);
        }
    }
}

template<Direction Direction>
void generateMovesForDirection(const State state, std::vector<State> &states)
{
    // Note: bottom boards are always homeboards
    generateMovesOnBoard<Direction>(BottomLeft, TopRight, state, states);
    generateMovesOnBoard<Direction>(BottomLeft, BottomRight, state, states);
    generateMovesOnBoard<Direction>(BottomRight, TopLeft, state, states);
    generateMovesOnBoard<Direction>(BottomRight, BottomLeft, state, states);
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
