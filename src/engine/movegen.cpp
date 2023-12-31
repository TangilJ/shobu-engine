#include <iostream>
#include "types.h"
#include "consts.h"
#include "print.h"

template <Direction Direction>
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

template <Direction Direction>
constexpr Board edge()
{
    // @formatter:off
    return Direction == Direction::Up        ? top
         : Direction == Direction::UpRight   ? top & right
         : Direction == Direction::Right     ? right
         : Direction == Direction::DownRight ? bottom & right
         : Direction == Direction::Down      ? bottom
         : Direction == Direction::DownLeft  ? bottom & left
         : Direction == Direction::Left      ? left
         : Direction == Direction::UpLeft    ? top & left
         : 0;
    // @formatter:on
}

template <Direction Direction>
void addPassiveMoves(const BoardType passiveBoard,
                     const State& state,
                     States& states)
{
    const Board ownStones = state.own[passiveBoard];
    const Board enemyStones = state.enemy[passiveBoard];

    // Go through each own stone on passive board
    // TODO: Abstract this behind an iterator for Board
    Board stonesLeft = ownStones;
    while (stonesLeft)
    {
        const Board currentStone = stonesLeft & -stonesLeft;
        stonesLeft ^= currentStone;

        // Passive move not allowed if it would push another stone        
        const Board moveOne = move<Direction>(currentStone);
        const bool blocked = (ownStones | enemyStones) & moveOne;

        // Passive move not allowed if it would push self off board.
        // move() returns 0 if pushing itself off so moveOne == 0 if on edge
        const bool onEdge = !moveOne;

        if (blocked || onEdge)
            continue;

        // Allowed to make passive move
        const Board ownBoardAfterPassive = ownStones ^ currentStone | moveOne;

        State newState = state;
        newState.own[passiveBoard] = ownBoardAfterPassive;
        states.add(newState);
    }
}

// TODO: Template this for all directions
// Returns 0 if aggressive move not allowed
template <Direction Direction>
TwoBoards applyStoneAggressiveMove(const Board ownStones,
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
        return 0;
#pragma endregion

#pragma region Apply aggressive move
    // Move own stone up. Remove current stone and place it one square above
    const Board ownStonesMoved = ownStones ^ stone | moveOne;

    // Enemy stone is pushed up if it exists. If there is an enemy stone one
    // square in next square, duplicate it up and remove its previous position
    const Board enemyStonesMoved =
        move<Direction>(moveOne & enemyStones)
        | enemyStones & ~moveOne;
#pragma endregion

    return static_cast<TwoBoards>(ownStonesMoved) << 16 | enemyStonesMoved;
}

template <Direction Direction>
void addBothMoves(const State state,
                  const BoardType aggressiveBoard,
                  const BoardType passiveBoard,
                  States& states)
{
    const Board ownStones = state.own[aggressiveBoard];
    const Board enemyStones = state.enemy[aggressiveBoard];

    // Go through each own stone on passive board
    Board stonesLeft = ownStones;
    while (stonesLeft)
    {
        const Board currentStone = stonesLeft & -stonesLeft;
        stonesLeft ^= currentStone;

#pragma region Add first move
        const TwoBoards aggrMoveResult = applyStoneAggressiveMove<Direction>(
            ownStones, enemyStones, currentStone
        );

        if (aggrMoveResult == 0)
            continue;

        // TODO: See if this can be wrapped
        const Board ownStonesMoved = aggrMoveResult >> 16;
        const Board enemyStonesMoved = static_cast<Board>(aggrMoveResult);

        State stateWithAggressiveMove = state;
        stateWithAggressiveMove.own[aggressiveBoard] = ownStonesMoved;
        stateWithAggressiveMove.enemy[aggressiveBoard] = enemyStonesMoved;

        addPassiveMoves<Direction>(
            passiveBoard, stateWithAggressiveMove, states
        );
#pragma endregion

#pragma region Add second move
        State stateAfterOneMove = state;
        stateAfterOneMove.own[aggressiveBoard] = ownStonesMoved;
        stateAfterOneMove.enemy[aggressiveBoard] = enemyStonesMoved;

        const TwoBoards aggrMoveResult2 = applyStoneAggressiveMove<Direction>(
            ownStonesMoved, enemyStonesMoved, currentStone
        );

        if (aggrMoveResult2 == 0)
            continue;

        const Board ownStonesAfterMove2 = aggrMoveResult2 >> 16;
        const Board enemyStonesAfterMove2 = static_cast<Board>(aggrMoveResult2);

        State stateWithAggressiveMove2 = stateAfterOneMove;
        stateWithAggressiveMove2.own[aggressiveBoard] = ownStonesAfterMove2;
        stateWithAggressiveMove2.enemy[aggressiveBoard] = enemyStonesAfterMove2;

        addPassiveMoves<Direction>(
            passiveBoard, stateWithAggressiveMove2, states
        );
#pragma endregion
    }
}

template <Direction Direction>
void addMovesForDirection(const State state, States& states)
{
    addBothMoves<Direction>(state, TopLeft, BottomRight, states);
    addBothMoves<Direction>(state, BottomLeft, BottomRight, states);
    addBothMoves<Direction>(state, TopRight, BottomLeft, states);
    addBothMoves<Direction>(state, BottomRight, BottomLeft, states);
}

void addMoves(const State state, States& states)
{
    addMovesForDirection<Direction::Up>(state, states);
    addMovesForDirection<Direction::Up>(state, states);
    addMovesForDirection<Direction::UpRight>(state, states);
    addMovesForDirection<Direction::Right>(state, states);
    addMovesForDirection<Direction::DownRight>(state, states);
    addMovesForDirection<Direction::Down>(state, states);
    addMovesForDirection<Direction::DownLeft>(state, states);
    addMovesForDirection<Direction::Left>(state, states);
    addMovesForDirection<Direction::UpLeft>(state, states);
}
