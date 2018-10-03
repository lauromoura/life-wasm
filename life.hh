#ifndef LIFE_HH
#define LIFE_HH


#include <vector>

class Game
{
public:
    Game(int rows, int cols) : rows(rows), cols(cols), board(rows * cols, false) {}

    int GetRows() const;
    int GetCols() const;
    const std::vector<bool>& GetBoard() const;

    bool At(const int row, const int col) const;
    void Activate(const int row, const int col);
    void Deactivate(const int row, const int col);

    void Evolve();
    void Print() const;

    inline int Translate(const int row, const int col) const;

private:

    void Set(const int row, const int col, bool value);

    int rows;
    int cols;
    std::vector<bool> board;
};

#endif
