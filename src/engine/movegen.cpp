#include <iostream>
#include "types.h"
#include "consts.h"
#include "print.h"

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

        // Passive move of up 1 not allowed if there is stone above
        const Board moveOne = currentStone << 4;
        const bool blocked = (ownStones | enemyStones) & moveOne;

        // Passive move of 1 up not allowed if it would push self off board
        const bool onEdge = top & currentStone;

        if (blocked || onEdge)
            continue;

        // Allowed to make passive move of 1 up
        const Board ownBoardAfterPassive = ownStones ^ currentStone | moveOne;

        State newState = state;
        newState.own[passiveBoard] = ownBoardAfterPassive;
        states.add(newState);
    }
}

// TODO: Template this for all directions
// Returns 0 if aggressive move not allowed
TwoBoards applyStoneAggressiveMove(const Board ownStones,
                                   const Board enemyStones,
                                   const Board stone)
{
#pragma region Move restrictions
    // Aggressive move of 1 up not allowed when pushing more than 1 stone
    const Board moveOne = stone << 4;
    const Board moveTwo = stone << 8;
    const Board blockPath = moveOne | moveTwo;
    const bool stonesInBlockPath =
        ((ownStones | enemyStones) & blockPath)
        == blockPath;

    // Aggressive move of 1 up not allowed when pushing own stone
    const bool pushingOwnStone = ownStones & moveOne;

    // Aggressive move of 1 up not allowed if it would push self off board
    const bool onEdge = top & stone;

    if (stonesInBlockPath || pushingOwnStone || onEdge)
        return 0;
#pragma endregion

#pragma region Apply aggressive move
    // Move own stone up. Remove current stone and place it one square above
    const Board ownStonesMoved = ownStones ^ stone | moveOne;

    // Enemy stone is pushed up if it exists. If there is an enemy stone one
    // square above, duplicate it up and remove its previous position
    const Board enemyStonesMoved =
        (moveOne & enemyStones) << 4
        | enemyStones & ~moveOne;
#pragma endregion

    return static_cast<TwoBoards>(ownStonesMoved) << 16 | enemyStonesMoved;
}

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
        const TwoBoards aggrMoveResult = applyStoneAggressiveMove(
            ownStones, enemyStones, currentStone);

        if (aggrMoveResult == 0)
            continue;

        // TODO: See if this can be wrapped
        const Board ownStonesMoved = aggrMoveResult >> 16;
        const Board enemyStonesMoved = static_cast<Board>(aggrMoveResult);

        State stateWithAggressiveMove = state;
        stateWithAggressiveMove.own[aggressiveBoard] = ownStonesMoved;
        stateWithAggressiveMove.enemy[aggressiveBoard] = enemyStonesMoved;

        addPassiveMoves(passiveBoard, stateWithAggressiveMove, states);
#pragma endregion

#pragma region Add second move
        State stateAfterOneMove = state;
        stateAfterOneMove.own[aggressiveBoard] = ownStonesMoved;
        stateAfterOneMove.enemy[aggressiveBoard] = enemyStonesMoved;

        const TwoBoards aggrMoveResult2 = applyStoneAggressiveMove(
            ownStonesMoved, enemyStonesMoved, currentStone);

        if (aggrMoveResult2 == 0)
            continue;

        const Board ownStonesAfterMove2 = aggrMoveResult2 >> 16;
        const Board enemyStonesAfterMove2 = static_cast<Board>(aggrMoveResult2);

        State stateWithAggressiveMove2 = stateAfterOneMove;
        stateWithAggressiveMove2.own[aggressiveBoard] = ownStonesAfterMove2;
        stateWithAggressiveMove2.enemy[aggressiveBoard] = enemyStonesAfterMove2;

        addPassiveMoves(passiveBoard, stateWithAggressiveMove2, states);
#pragma endregion
    }
}

void addMoves(const State state, States& states)
{
    addBothMoves(state, TopLeft, BottomRight, states);
    addBothMoves(state, BottomLeft, BottomRight, states);
    addBothMoves(state, TopRight, BottomLeft, states);
    addBothMoves(state, BottomRight, BottomLeft, states);
}