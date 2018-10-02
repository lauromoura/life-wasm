
#include <SDL2/SDL.h>

#include <vector>
#include <iostream>


class Game
{
public:
    Game(int rows, int cols) : rows(rows), cols(cols), board(rows * cols, false) {
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
    const std::vector<bool>& getBoard() const { return board; }

    bool At(const int row, const int col) const
    {
        const long idx = row * cols + col;
        if (idx < rows * cols && idx >= 0)
            return board[idx];
        else
            return false;
    }

    void Activate(const int row, const int col)
    {
        Set(row, col, true);
    }

    void Deactivate(const int row, const int col)
    {
        Set(row, col, false);
    }

    void Evolve()
    {
        std::vector<bool> current = board;
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

                        if (current[r * rows + c])
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

    void Print() const
    {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                bool val = At(row, col);

                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }

private:

    void Set(const int row, const int col, bool value)
    {
        const long idx = row *cols + col;
        if (idx >= rows * cols || idx < 0)
            return; //FIXME Message out of bounds error

        board[idx] = value;
    }


    int rows;
    int cols;
    std::vector<bool> board;
};

int main(int argc, char *argv[])
{
    Game g(5, 5);
    g.Activate(0, 1);
    g.Activate(1, 1);
    g.Activate(2, 1);
    g.Activate(3, 1);
    g.Deactivate(3, 1);
    g.Print();
    for (int i = 0; i < 10; i++) {
        std::cout << "Iteration: " << i << std::endl;
        g.Evolve();
        g.Print();
    }
    return 0;
}
