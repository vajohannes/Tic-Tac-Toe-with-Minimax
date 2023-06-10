#include "board.h"
using namespace std;

Board::Board(const int score[][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            cells[i][j] = EMPTY;
            this->score[i][j] = score[i][j];
        }
    }
    curPlayer = X;
    id = 0;
}

bool Board::isFull() const
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (cells[i][j] == EMPTY)
            {
                return false;
            }
        }
    }
    return true;
}

bool Board::isFinished() const
{
    if (isFull())
    {
        return true;
    }
    int boardScore = getBoardScore();
    return boardScore == WIN_SCORE || boardScore == -WIN_SCORE;
}

int Board::getBoardScore() const
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        Cell rowCheck = cells[i][0];
        for (int j = 1; j < BOARD_SIZE; j++)
        {
            if (rowCheck != cells[i][j])
            {
                rowCheck = EMPTY;
                break;
            }
        }
        if (rowCheck == EMPTY)
        {
            continue;
        }
        return (rowCheck == X) ? WIN_SCORE : -WIN_SCORE;
    }

    for (int j = 0; j < BOARD_SIZE; j++)
    {
        Cell colCheck = cells[0][j];
        for (int i = 1; i < BOARD_SIZE; i++)
        {
            if (colCheck != cells[i][j])
            {
                colCheck = EMPTY;
                break;
            }
        }
        if (colCheck == EMPTY)
        {
            continue;
        }
        return (colCheck == X) ? WIN_SCORE : -WIN_SCORE;
    }

    Cell diagDownCheck = cells[0][0];
    for (int i = 1; i < BOARD_SIZE; i++)
    {
        if (diagDownCheck != cells[i][i])
        {
            diagDownCheck = EMPTY;
            break;
        }
    }

    if (diagDownCheck != EMPTY)
    {
        return (diagDownCheck == X) ? WIN_SCORE : -WIN_SCORE;
    }

    Cell diagUpCheck = cells[0][BOARD_SIZE - 1];
    for (int i = 1; i < BOARD_SIZE; i++)
    {
        if (diagUpCheck != cells[i][BOARD_SIZE - i - 1])
        {
            diagUpCheck = EMPTY;
            break;
        }
    }

    if (diagUpCheck != EMPTY)
    {
        return (diagUpCheck == X) ? WIN_SCORE : -WIN_SCORE;
    }

    int total = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            total += score[i][j] * static_cast<int>(cells[i][j]) * getCellWeight(cells[i][j]);
        }
    }
    return total;
}

bool Board::play(const BoardCoordinate &coords)
{
    if (!coords.isValid())
    {
        return false;
    }
    if (cells[coords.row][coords.col] != EMPTY)
    {
        return false;
    }
    cells[coords.row][coords.col] = curPlayer;
    id += ((static_cast<int>(curPlayer) + 3) % 3) * pow(3, (coords.row * BOARD_SIZE + coords.col));
    curPlayer = static_cast<Cell>(-curPlayer);
    return true;
}