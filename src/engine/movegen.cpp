#include <intrin.h>
#include "movegen.h"
#include "types.h"
#include "consts.h"

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

// If any one of a player's 4x4 boards has no pieces left, the player loses
Win checkWin(const Board board)
{
    for (const Quarter quarter: {board.topLeft, board.topRight,
                                 board.bottomLeft, board.bottomRight})
    {
        if (quarter.own == 0)
            return Win::OpponentWin;
        if (quarter.enemy == 0)
            return Win::OwnWin;
    }

    return Win::GameOngoing;
}

// Is the target allowed to be where it is, given the current quarter?
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
                          Moves<Bitboard> &moveOnes,
                          Moves<Bitboard> &moveTwos)
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
        int sourceStoneIndex = 15 - _tzcnt_u32(stone);
        moveOnes.add(ownAfterPassive, sourceStoneIndex);

        // Second passive move if first move was allowed
        const Bitboard moveTwo = move<direction>(moveOne);
        const bool allowed2 = isPassiveMoveLegal(
            moveTwo, ownAfterPassive, quarter.enemy
        );
        if (!allowed2)
            continue;

        Bitboard ownAfterPassive2 = ownAfterPassive ^ moveOne | moveTwo;
        moveTwos.add(ownAfterPassive2, sourceStoneIndex);
    }
}

template<Direction direction>
void generateAggressiveMoves(const Quarter quarter,
                             Moves<Quarter> &moveOnes,
                             Moves<Quarter> &moveTwos)
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
        int sourceStoneIndex = 15 - _tzcnt_u32(stone);
        moveOnes.add(aggr1, sourceStoneIndex);


        // Second aggressive move if first move was allowed
        const Bitboard moveOne = move<direction>(stone);
        bool legal2 = isAggressiveMoveLegal<direction>(aggr1, moveOne);
        if (!legal2) continue;

        const Quarter aggr2 = applyAggressiveMove<direction>(
            aggr1,
            moveOne
        );
        moveTwos.add(aggr2, sourceStoneIndex);
    }
}


template<Direction direction, int times, Location passiveBoard, Location aggressiveBoard>
void combinePassiveAggressive(const Board &board,
                              std::vector<State> &states,
                              Moves<Bitboard> &passiveMoves,
                              Moves<Quarter> &aggressiveMoves)
{
    for (int i = 0; i < aggressiveMoves.size(); ++i)
    {
        for (int j = 0; j < passiveMoves.size(); ++j)
        {
            Quarter newPassive = {
                passiveMoves[j],
                board.getQuarter<passiveBoard>().enemy
            };

            Board newBoard = board.setQuarter<passiveBoard>(newPassive);
            newBoard = newBoard.setQuarter<aggressiveBoard>(aggressiveMoves[i]);
            newBoard = reverseBoard(newBoard);
            Move move = {
                passiveMoves.getSourceIndex(j),
                aggressiveMoves.getSourceIndex(i),
                passiveBoard == Location::BottomLeft ? PassiveSide::Left : PassiveSide::Right,
                aggressiveBoard == Location::TopLeft ? AggressiveSide::Top : AggressiveSide::Bottom,
                direction,
                times
            };
            Win win = checkWin(newBoard);

            states.push_back({newBoard, move, win});
        }
    }
}

template<Direction direction, Location passiveBoard, Location aggressiveBoard>
void generateMovesOnBoard(const Board &board,
                          std::vector<State> &states)
{
    Moves<Bitboard> passiveMoveOnes = {};
    Moves<Bitboard> passiveMoveTwos = {};
    generatePassiveMoves<direction>(
        board.getQuarter<passiveBoard>(),
        passiveMoveOnes,
        passiveMoveTwos
    );

    Moves<Quarter> aggressiveMoveOnes = {};
    Moves<Quarter> aggressiveMoveTwos = {};
    generateAggressiveMoves<direction>(
        board.getQuarter<aggressiveBoard>(),
        aggressiveMoveOnes,
        aggressiveMoveTwos
    );

    combinePassiveAggressive<direction, 1, passiveBoard, aggressiveBoard>(
        board, states, passiveMoveOnes, aggressiveMoveOnes
    );
    combinePassiveAggressive<direction, 2, passiveBoard, aggressiveBoard>(
        board, states, passiveMoveTwos, aggressiveMoveTwos
    );
}

template<Direction direction>
void generateMovesForDirection(const Board board,
                               std::vector<State> &states)
{
    // Note: bottom states are always homeboards
    generateMovesOnBoard<direction, Location::BottomLeft, Location::TopRight>(board, states);
    generateMovesOnBoard<direction, Location::BottomLeft, Location::BottomRight>(board, states);
    generateMovesOnBoard<direction, Location::BottomRight, Location::TopLeft>(board, states);
    generateMovesOnBoard<direction, Location::BottomRight, Location::BottomLeft>(board, states);
}

void generateAllMovesInPly(const Board board,
                           std::vector<State> &states)
{
    generateMovesForDirection<Direction::Up>(board, states);
    generateMovesForDirection<Direction::UpRight>(board, states);
    generateMovesForDirection<Direction::Right>(board, states);
    generateMovesForDirection<Direction::DownRight>(board, states);
    generateMovesForDirection<Direction::Down>(board, states);
    generateMovesForDirection<Direction::DownLeft>(board, states);
    generateMovesForDirection<Direction::Left>(board, states);
    generateMovesForDirection<Direction::UpLeft>(board, states);
}

Bitboard reverseBitboard(Bitboard x)
{
    // @formatter:off
    // Adapted from:
    // https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating
    const Bitboard x1 = 0b0101'0101'0101'0101;
    const Bitboard x2 = 0b0011'0011'0011'0011;
    const Bitboard x4 = 0b0000'1111'0000'1111;
    x = ((x >> 1) & x1) | ((x & x1) << 1); // Swap adjacent bits
    x = ((x >> 2) & x2) | ((x & x2) << 2); // Swap pairs
    x = ((x >> 4) & x4) | ((x & x4) << 4); // Swap nibbles
    x = (x >> 8)        | (x << 8);        // Swap bytes
    return x;
    // @formatter:on
}

Quarter reverseQuarter(const Quarter &quarter)
{
    return {
        reverseBitboard(quarter.enemy),
        reverseBitboard(quarter.own)
    };
}

Board reverseBoard(const Board &board)
{
    return {
        {reverseQuarter(board.bottomRight)},
        {reverseQuarter(board.bottomLeft)},
        {reverseQuarter(board.topRight)},
        {reverseQuarter(board.topLeft)},
    };
}