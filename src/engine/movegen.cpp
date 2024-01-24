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

bool passiveMoveAllowed(Board target, Board ownStones, Board enemyStones)
{
    // Passive move not allowed if it would push another stone
    const bool blocked = (ownStones | enemyStones) & target;

    // Passive move not allowed if it would push self off board.
    // move() returns 0 if pushing itself off so moveOne == 0 if on edge
    const bool onEdge = !target;

    return !(blocked || onEdge);
}

template<Direction Direction>
void addPassiveMoves(const BoardType passiveBoard,
                     const BoardType aggressiveBoard,
                     const State &aggr1,
                     const State &aggr2,
                     States &states)
{
    const Board ownStones = aggr1.own[passiveBoard];
    const Board enemyStones = aggr1.enemy[passiveBoard];

    // Go through each own stone on passive board
    // TODO: Abstract this behind an iterator for Board
    Board stonesLeft = ownStones;
    while (stonesLeft) {
        const Board currentStone = stonesLeft & -stonesLeft;
        stonesLeft ^= currentStone;

        // Passive move when aggressive moves 1 square
        const Board moveOne = move<Direction>(currentStone);
        const bool passive1Allowed = passiveMoveAllowed(
            moveOne, ownStones, enemyStones
        );
        if (!passive1Allowed)
            continue;

        Board ownAfterPassive1 = ownStones ^ currentStone | moveOne;
        State newState = aggr1;
        newState.own[passiveBoard] = ownAfterPassive1;
        states.add(newState);

        // Passive move when aggressive moves 2 squares
        if (aggr2.own[aggressiveBoard] == 0)
            // Empty board means no 2nd aggressive move
            continue;

        const Board moveTwo = move<Direction>(moveOne);
        const bool passive2Allowed = passiveMoveAllowed(
            moveTwo, ownAfterPassive1, enemyStones
        );
        if (!passive2Allowed)
            continue;

        Board ownAfterPassive2 = ownAfterPassive1 ^ moveOne | moveTwo;
        newState = aggr2;
        newState.own[passiveBoard] = ownAfterPassive2;
        states.add(newState);
    }
}

// Returns 0 if aggressive move not allowed
template<Direction Direction>
BoardPair aggressiveMove(const Board ownStones,
                         const Board enemyStones,
                         const Board stone)
{
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
void addBothMoves(const State state,
                  const BoardType aggressiveBoard,
                  const BoardType passiveBoard,
                  States &states)
{
    const Board ownStones = state.own[aggressiveBoard];
    const Board enemyStones = state.enemy[aggressiveBoard];

    // Go through each own stone on passive board
    Board stonesLeft = ownStones;
    while (stonesLeft) {
        const Board stone = stonesLeft & -stonesLeft;
        stonesLeft ^= stone;

        // First aggressive move
        const BoardPair aggr1 = aggressiveMove<Direction>(
            ownStones, enemyStones, stone
        );
        if (aggr1.isEmpty())
            continue;

        State aggr1State = state;
        aggr1State.own[aggressiveBoard] = aggr1.own;
        aggr1State.enemy[aggressiveBoard] = aggr1.enemy;

        // Second aggressive move
        const Board moveOne = move<Direction>(stone);
        const BoardPair aggr2 = aggressiveMove<Direction>(
            aggr1.own, aggr1.enemy, moveOne
        );

        State aggr2State = state;
        aggr2State.own[aggressiveBoard] = aggr2.own;
        aggr2State.enemy[aggressiveBoard] = aggr2.enemy;

        // Add passive moves to both aggressive moves
        addPassiveMoves<Direction>(
            passiveBoard, aggressiveBoard, aggr1State, aggr2State, states
        );
    }
}

template<Direction Direction>
void addMovesForDirection(const State state, States &states)
{
    // Note: bottom boards are always homeboards
    addBothMoves<Direction>(state, TopLeft, BottomRight, states);
    addBothMoves<Direction>(state, BottomLeft, BottomRight, states);
    addBothMoves<Direction>(state, TopRight, BottomLeft, states);
    addBothMoves<Direction>(state, BottomRight, BottomLeft, states);
}

void addMoves(const State state, States &states)
{
    addMovesForDirection<Direction::Up>(state, states);
    addMovesForDirection<Direction::UpRight>(state, states);
    addMovesForDirection<Direction::Right>(state, states);
    addMovesForDirection<Direction::DownRight>(state, states);
    addMovesForDirection<Direction::Down>(state, states);
    addMovesForDirection<Direction::DownLeft>(state, states);
    addMovesForDirection<Direction::Left>(state, states);
    addMovesForDirection<Direction::UpLeft>(state, states);
}
