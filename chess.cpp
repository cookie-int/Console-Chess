#include <iostream>
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

// base class
class Piece
{
protected:
    // position
    int row;
    int col;
    int power;
    Colour colour;
    string symbol;

public:
    Piece(int row, int col, Colour colour, string symbol)
        : row(row), col(col), colour(colour), symbol(symbol) {}

    // getters
    string getSymbol()
    {
        return symbol;
    }
    Colour getColor()
    {
        return colour;
    }

    // pure virtual function
    virtual bool isValidMove(Piece *grid[8][8], Move m) = 0;

    virtual ~Piece() {}
};

class Pawn : public Piece
{
public:
    Pawn(int row, int col, Colour colour) : Piece(row, col, colour, colour == WHITE ? "♙ " : "♟ ") {}
    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        return 0;
    }
};
class Rook : public Piece
{
public:
    Rook(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♖ " : "♜ ") {}
    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        return 0;
    }
};

class Knight : public Piece
{
public:
    Knight(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♘ " : "♞ ") {}
    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        return 0;
    }
};

class Bishop : public Piece
{
public:
    Bishop(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♗ " : "♝ ") {}
    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        return 0;
    }
};

class Queen : public Piece
{
public:
    Queen(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♕ " : "♛ ") {}
    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        return 0;
    }
};

class King : public Piece
{
public:
    King(int row, int col, Colour colour)
        : Piece(row, col, colour, colour == WHITE ? "♔ " : "♚ ") {}
    bool isValidMove(Piece *grid[8][8], Move m) override
    {
        return 0;
    }
};

void initializeBoard(Piece *grid[8][8])
{
    // empty board
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            grid[i][j] = nullptr;

    // pawns
    for (int j = 0; j < 8; j++)
    {
        grid[1][j] = new Pawn(1, j, BLACK);
        grid[6][j] = new Pawn(6, j, WHITE);
    }

    // rooks
    grid[0][0] = new Rook(0, 0, BLACK);
    grid[0][7] = new Rook(0, 7, BLACK);
    grid[7][0] = new Rook(7, 0, WHITE);
    grid[7][7] = new Rook(7, 7, WHITE);

    // knights
    grid[0][1] = new Knight(0, 1, BLACK);
    grid[0][6] = new Knight(0, 6, BLACK);
    grid[7][1] = new Knight(7, 1, WHITE);
    grid[7][6] = new Knight(7, 6, WHITE);

    // bishops
    grid[0][2] = new Bishop(0, 2, BLACK);
    grid[0][5] = new Bishop(0, 5, BLACK);
    grid[7][2] = new Bishop(7, 2, WHITE);
    grid[7][5] = new Bishop(7, 5, WHITE);

    // queens
    grid[0][3] = new Queen(0, 3, BLACK);
    grid[7][3] = new Queen(7, 3, WHITE);

    // kings
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

            // background colors
            if (isLight)
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE); // light (white)
            else
                SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY); // dark (gray)

            if (grid[i][j] == nullptr)
                cout << "    ";
            else
                cout << " " << grid[i][j]->getSymbol() << " ";
        }

        // reset after each row
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
