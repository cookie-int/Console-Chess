#include <iostream>
#include <cmath>
#include <windows.h>
using namespace std;

// since there are only 2 possible colours we use enum
enum Colour
{
    WHITE,
    BLACK
};

// to store move current and destination coordinates
struct Move
{
    int sr, sc; // source row, col
    int dr, dc; // destination row, col
};

struct Position
{
    int r, c;
};

class Piece
{
protected:
    int row;
    int col;
    int power;
    Colour colour;
    string symbol;

public:
    Piece(int row, int col, Colour colour, string symbol)
        : row(row), col(col), colour(colour), symbol(symbol) {}

    string getSymbol() { return symbol; }
    Colour getColor() { return colour; }

    virtual bool isValidMove(Piece *grid[8][8], Move m) = 0;

    virtual ~Piece() {}
};

// pawn moves forward 1, forward 2 from start row, or captures diagonally
// direction depends on colour: white goes up (-1), black goes down (+1)
class Pawn : public Piece
{
public:
    Pawn(int row, int col, Colour colour) : Piece(row, col, colour, colour == WHITE ? "♙ " : "♟ ") {}

    int dir() const
    {
        return (colour == WHITE) ? -1 : 1;
    }

    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        int sr = m.sr, sc = m.sc;
        int dr = m.dr, dc = m.dc;
        Piece *dest = grid[dr][dc];

        // forward 1
        if (dc == sc && dest == nullptr && dr == sr + dir())
        {
            return true;
        }

        // forward 2 from starting row
        if ((colour == WHITE && sr == 6) || (colour == BLACK && sr == 1))
        {
            if (dc == sc &&
                dr == sr + 2 * dir() &&
                grid[sr + dir()][sc] == nullptr &&
                dest == nullptr)
            {
                return true;
            }
        }

        // diagonal capture: opponent piece must be present
        if (abs(static_cast<int>(dc - sc)) == 1 && dr == sr + dir() && dest != nullptr && dest->getColor() != colour)
        {
            return true;
        }

        return false;
    }
};

// rook moves any number of squares along a row or column, path must be clear
class Rook : public Piece
{
public:
    Rook(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♖ " : "♜ ") {}

    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        int sr = m.sr, sc = m.sc;
        int dr = m.dr, dc = m.dc;

        if (sr != dr && sc != dc)
        {
            return false;
        }

        int rStep = (dr > sr) ? 1 : (dr < sr ? -1 : 0);
        int cStep = (dc > sc) ? 1 : (dc < sc ? -1 : 0);

        int r = sr + rStep;
        int c = sc + cStep;

        while (r != dr || c != dc)
        {
            if (grid[r][c] != nullptr)
            {
                return false;
            }
            r += rStep;
            c += cStep;
        }

        return (grid[dr][dc] == nullptr || grid[dr][dc]->getColor() != colour);
    }
};

// knight moves in an l-shape (2+1), jumps over pieces
class Knight : public Piece
{
public:
    Knight(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♘ " : "♞ ") {}

    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        int drAbs = abs(m.dr - m.sr);
        int dcAbs = abs(m.dc - m.sc);

        if (!((drAbs == 2 && dcAbs == 1) || (drAbs == 1 && dcAbs == 2)))
        {
            return false;
        }

        return (grid[m.dr][m.dc] == nullptr || grid[m.dr][m.dc]->getColor() != colour);
    }
};

// bishop moves diagonally any number of squares, path must be clear
class Bishop : public Piece
{
public:
    Bishop(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♗ " : "♝ ") {}

    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        int sr = m.sr, sc = m.sc;
        int dr = m.dr, dc = m.dc;

        if (abs(dr - sr) != abs(dc - sc))
        {
            return false;
        }

        int rStep = (dr > sr) ? 1 : -1;
        int cStep = (dc > sc) ? 1 : -1;

        int r = sr + rStep;
        int c = sc + cStep;

        while (r != dr)
        {
            if (grid[r][c] != nullptr)
            {
                return false;
            }
            r += rStep;
            c += cStep;
        }

        return (grid[dr][dc] == nullptr || grid[dr][dc]->getColor() != colour);
    }
};

// queen combines rook and bishop movement
class Queen : public Piece
{
public:
    Queen(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♕ " : "♛ ") {}

    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        Rook rook(0, 0, colour);
        Bishop bishop(0, 0, colour);
        return rook.isValidMove(grid, m) || bishop.isValidMove(grid, m);
    }
};

// king moves exactly 1 square in any direction
class King : public Piece
{
public:
    King(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♔ " : "♚ ") {}

    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        int dr = abs(m.dr - m.sr);
        int dc = abs(m.dc - m.sc);

        if (dr > 1 || dc > 1)
        {
            return false;
        }

        return (grid[m.dr][m.dc] == nullptr || grid[m.dr][m.dc]->getColor() != colour);
    }
};

void initializeBoard(Piece *grid[8][8])
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = nullptr;

    for (int j = 0; j < 8; j++)
    {
        grid[1][j] = new Pawn(1, j, BLACK);
        grid[6][j] = new Pawn(6, j, WHITE);
    }

    grid[0][0] = new Rook(0, 0, BLACK);
    grid[0][7] = new Rook(0, 7, BLACK);
    grid[7][0] = new Rook(7, 0, WHITE);
    grid[7][7] = new Rook(7, 7, WHITE);

    grid[0][1] = new Knight(0, 1, BLACK);
    grid[0][6] = new Knight(0, 6, BLACK);
    grid[7][1] = new Knight(7, 1, WHITE);
    grid[7][6] = new Knight(7, 6, WHITE);

    grid[0][2] = new Bishop(0, 2, BLACK);
    grid[0][5] = new Bishop(0, 5, BLACK);
    grid[7][2] = new Bishop(7, 2, WHITE);
    grid[7][5] = new Bishop(7, 5, WHITE);

    grid[0][3] = new Queen(0, 3, BLACK);
    grid[7][3] = new Queen(7, 3, WHITE);

    grid[0][4] = new King(0, 4, BLACK);
    grid[7][4] = new King(7, 4, WHITE);
}

void displayBoard(Piece *grid[8][8])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 0; i < 8; i++)
    {
        cout << 8 - i << " ";

        for (int j = 0; j < 8; j++)
        {
            bool isLight = (i + j) % 2 == 0;

            if (isLight)
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
            else
                SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY);

            if (grid[i][j] == nullptr)
                cout << "    ";
            else
                cout << " " << grid[i][j]->getSymbol() << " ";
        }

        SetConsoleTextAttribute(hConsole, 7);
        cout << endl;
    }

    cout << "   a   b   c   d   e   f   g   h\n";
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Piece *grid[8][8];
    initializeBoard(grid);
    displayBoard(grid);

    return 0;
}
