
#include <SDL2/SDL.h>

#include <iostream>

#include "life.hh"


int Game::GetRows() const { return rows; }
int Game::GetCols() const { return cols; }
const std::vector<bool>& Game::GetBoard() const { return board; }



bool Game::At(const int row, const int col, Generation generation) const
{
    const long idx = row * cols + col;
    if (idx < rows * cols && idx >= 0)
        return (generation == Current) ? board[idx] : previous[idx];
    else
        return false;
}

void Game::Activate(const int row, const int col)
{
    Set(row, col, true);
}

void Game::Deactivate(const int row, const int col)
{
    Set(row, col, false);
}

void Game::Toggle(const int row, const int col)
{
    Set(row, col, !At(row, col));
}

void Game::Evolve()
{
    previous = board;
    // Create new board
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int neighbours_alive = 0;

            for (int dr=-1; dr <= 1; dr++) {
                for (int dc=-1; dc <= 1; dc++) {

                    // Skip self
                    if (dr == 0 && dc == 0)
                        continue;

                    const int r = row + dr;
                    const int c = col + dc;

                    if (r < 0 || c < 0)
                        continue;

                    if (r >= rows || c >= cols)
                        continue;

                    if (previous[r * rows + c])
                        neighbours_alive++;
                }
            }

            if (neighbours_alive < 2 || neighbours_alive > 3)
                Deactivate(row, col);
            else if (neighbours_alive == 3)
                Activate(row, col);
        }
    }
}

void Game::Print() const
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            bool val = At(row, col);

            std::cout << (val ? "#" : "_") << " ";
        }
        std::cout << std::endl;
    }
}

void Game::Set(const int row, const int col, bool value)
{
    const long idx = row *cols + col;
    if (idx >= rows * cols || idx < 0)
        return; //FIXME Message out of bounds error

    board[idx] = value;
}

inline int Game::Translate(const int row, const int col) const {
    return row * cols + col;
}
