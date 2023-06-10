#include "boardtree.h"

BoardTree::BoardTree(const Board &board)
{
    root = new BoardNode(board);
}

BoardTree::~BoardTree()
{
    delete root;
}

BoardTree *BoardTree::getSubTree(const BoardCoordinate &coords)
{
    if (root->subTree[coords.row][coords.col].isEmpty())
    {
        Board nextBoard = Board(root->board);
        if (nextBoard.play(BoardCoordinate(coords.row, coords.col)))
        {
            root->subTree[coords.row][coords.col].root = new BoardNode(nextBoard);
        }
    }
    return &(root->subTree[coords.row][coords.col]);
}

BoardOptimalMove BoardTree::getOptimalMove(const unsigned int depth)
{
    if (isEmpty())
    {
        return BoardOptimalMove();
    }

    BoardOptimalMove optimalMove;

    int boardScore = root->board.getBoardScore();

    if (depth == 0 || root->board.isFinished())
    {
        return BoardOptimalMove{boardScore, BoardCoordinate{-1, -1}};
    }

    BoardOptimalMove hashedMove = BoardHashTable::getInstance().getHashedMove(root->board.getID(), depth);
    if (hashedMove.score != ILLEGAL)
    {
        return hashedMove;
    }

    int optimalScore = root->board.getCurPlayer() == X ? -WIN_SCORE - 1 : WIN_SCORE + 1;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (root->subTree[i][j].isEmpty())
            {
                Board nextBoard = Board(root->board);
                if (nextBoard.play(BoardCoordinate(i, j)))
                {
                    root->subTree[i][j].root = new BoardNode(nextBoard);
                }
            }

            BoardOptimalMove childOpt = root->subTree[i][j].getOptimalMove(depth - 1);

            if (childOpt.score == ILLEGAL)
            {
                continue;
            }

            if (root->board.getCurPlayer() == X ? childOpt.score > optimalScore : childOpt.score < optimalScore)
            {
                optimalScore = childOpt.score;
                optimalMove = BoardOptimalMove{optimalScore, BoardCoordinate{i, j}};
            }
        }
    }

    BoardHashTable::getInstance().updateTable(root->board.getID(), depth, optimalMove);
    return optimalMove;
}

BoardOptimalMove BoardTree::getOptimalMoveAlphaBeta(const unsigned int depth, int alpha, int beta)
{
    if (isEmpty())
    {
        return BoardOptimalMove();
    }

    BoardOptimalMove optimalMove;

    int boardScore = root->board.getBoardScore();

    if (depth == 0 || root->board.isFinished())
    {
        return BoardOptimalMove{boardScore, BoardCoordinate{-1, -1}};
    }

    int optimalScore = root->board.getCurPlayer() == X ? -WIN_SCORE - 1 : WIN_SCORE + 1;
    int i = 0, j = 0;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        bool exit = false;
        for (j = 0; j < BOARD_SIZE; j++)
        {
            if (root->subTree[i][j].isEmpty())
            {
                Board nextBoard = Board(root->board);
                if (nextBoard.play(BoardCoordinate(i, j)))
                {
                    root->subTree[i][j].root = new BoardNode(nextBoard);
                }
            }

            BoardOptimalMove childOpt = root->subTree[i][j].getOptimalMoveAlphaBeta(depth - 1, alpha, beta);

            if (childOpt.score == ILLEGAL)
            {
                continue;
            }

            if (root->board.getCurPlayer() == X ? childOpt.score > optimalScore : childOpt.score < optimalScore)
            {
                optimalScore = childOpt.score;
                optimalMove = BoardOptimalMove{optimalScore, BoardCoordinate{i, j}};
            }

            if (root->board.getCurPlayer() == X)
            {
                if (optimalScore > beta)
                {
                    exit = true;
                    break;
                }
                alpha = optimalScore > alpha ? optimalScore : alpha;
            }
            else
            {
                if (optimalScore < alpha)
                {
                    exit = true;
                    break;
                }
                beta = optimalScore < beta ? optimalScore : beta;
            }
        }
        if (exit)
        {
            break;
        }
    }
    return optimalMove;
}