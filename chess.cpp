#include <iostream>
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
    virtual ~Piece() {}
};
