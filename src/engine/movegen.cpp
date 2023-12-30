#include <iostream>
#include "types.h"
#include "consts.h"
#include "print.h"

void addPassiveMoves(BoardType passiveBoard, const State &state, States &states)
{
    const Board ownStones = state.own[passiveBoard];
    const Board enemyStones = state.enemy[passiveBoard];

    // Go through each own stone on passive board
    Board stonesLeft = ownStones;
    while (stonesLeft) {
        const Board currentStone = stonesLeft & -stonesLeft;
        stonesLeft ^= currentStone;

        // Passive move of up 1 not allowed if there is stone above
        const Board moveOne = currentStone << 4;
        bool blocked = (ownStones | enemyStones) & moveOne;

        // Passive move of 1 up not allowed if it would push self off board
        bool onEdge = top & currentStone;

        if (blocked || onEdge)
            continue;

        // Allowed to make passive move of 1 up
        Board ownBoardAfterPassive = ownStones ^ currentStone | moveOne;

        State newState = state;
        newState.own[passiveBoard] = ownBoardAfterPassive;
        states.add(newState);
    }
}

// TODO: Template this for all directions
// Returns 0 if aggressive move not allowed
TwoBoards addStoneAggressiveMove(Board ownAggrStones,
                                 Board enemyAggrStones,
                                 Board stone)
{
#pragma region Move restrictions
    // Aggressive move of 1 up not allowed when pushing more than 1 stone
    Board moveOne = stone << 4;
    Board moveTwo = stone << 8;
    Board blockPath = moveOne | moveTwo;
    bool stonesInBlockPath = ((ownAggrStones | enemyAggrStones) & blockPath)
                             == blockPath;

    // Aggressive move of 1 up not allowed when pushing own stone
    bool pushingOwnStone = ownAggrStones & moveOne;

    // Aggressive move of 1 up not allowed if it would push self off board
    bool onEdge = top & stone;

    if (stonesInBlockPath || pushingOwnStone || onEdge)
        return 0;
#pragma endregion

#pragma region Apply aggressive move
    // Move own stone up. Remove current stone and place it one square above
    ownAggrStones = ownAggrStones ^ stone | moveOne;

    // Enemy stone is pushed up if it exists. If there is an enemy stone one
    // square above, duplicate it up and remove its previous position
    enemyAggrStones = (moveOne & enemyAggrStones) << 4
                      | enemyAggrStones & ~moveOne;
#pragma endregion

    return (TwoBoards) ownAggrStones << 16 | enemyAggrStones;
}

void addBothMoves(State state,
                  BoardType aggressiveBoard,
                  BoardType passiveBoard,
                  States &states)
{
    Board ownStones = state.own[aggressiveBoard];
    Board enemyStones = state.enemy[aggressiveBoard];

    // Go through each own stone on passive board
    while (ownStones) {
        Board currentStone = ownStones & -ownStones;
        ownStones ^= currentStone;

#pragma region Add first move
        TwoBoards aggrMoveResult = addStoneAggressiveMove(
            ownStones, enemyStones, currentStone);

        if (aggrMoveResult == 0)
            continue;

        // TODO: See if this can be wrapped
        Board ownStonesAfterMove = aggrMoveResult >> 16;
        Board enemyStonesAfterMove = (Board) aggrMoveResult;

        State stateWithAggressiveMove = state;
        state.own[aggressiveBoard] = ownStonesAfterMove;
        state.enemy[aggressiveBoard] = enemyStonesAfterMove;

        addPassiveMoves(passiveBoard, stateWithAggressiveMove, states);
#pragma endregion

#pragma region Add second move
        State stateAfterOneMove = state;
        stateAfterOneMove.own[aggressiveBoard] = ownStonesAfterMove;
        stateAfterOneMove.enemy[aggressiveBoard] = enemyStonesAfterMove;

        TwoBoards aggrMoveResult2 = addStoneAggressiveMove(
            ownStonesAfterMove, enemyStonesAfterMove, currentStone);

        if (aggrMoveResult2 == 0)
            continue;

        Board ownStonesAfterMove2 = aggrMoveResult2 >> 16;
        Board enemyStonesAfterMove2 = (Board) aggrMoveResult2;

        State stateWithAggressiveMove2 = stateAfterOneMove;
        stateWithAggressiveMove2.own[aggressiveBoard] = ownStonesAfterMove2;
        stateWithAggressiveMove2.enemy[aggressiveBoard] = enemyStonesAfterMove2;

        addPassiveMoves(passiveBoard, stateWithAggressiveMove2, states);
#pragma endregion
    }
}

void addMoves(State state, States &states)
{
    addBothMoves(state, TopLeft, BottomRight, states);
    addBothMoves(state, BottomLeft, BottomRight, states);
    addBothMoves(state, TopRight, BottomLeft, states);
    addBothMoves(state, BottomRight, BottomLeft, states);
}