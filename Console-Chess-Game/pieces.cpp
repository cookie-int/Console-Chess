#include "pieces.h"
#include "game.h"

Piece::Piece(int row, int col, Colour colour, string symbol)
    : row(row), col(col), colour(colour), symbol(symbol) {}

string Piece::getSymbol() { return symbol; }
Colour Piece::getColor() { return colour; }

Pawn::Pawn(int row, int col, Colour colour)
    : Piece(row, col, colour, colour == WHITE ? "♙ " : "♟ ") {}

int Pawn::dir() const
{
    return (colour == WHITE) ? -1 : 1;
}

bool Pawn::isValidMove(Piece *grid[8][8], Move m)
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

Rook::Rook(int row, int col, Colour colour)
    : Piece(row, col, colour, colour == WHITE ? "♖ " : "♜ ") {}

bool Rook::isValidMove(Piece *grid[8][8], Move m)
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

Knight::Knight(int row, int col, Colour colour)
    : Piece(row, col, colour, colour == WHITE ? "♘ " : "♞ ") {}

bool Knight::isValidMove(Piece *grid[8][8], Move m)
{
    int drAbs = abs(m.dr - m.sr);
    int dcAbs = abs(m.dc - m.sc);

    if (!((drAbs == 2 && dcAbs == 1) || (drAbs == 1 && dcAbs == 2)))
    {
        return false;
    }

    return (grid[m.dr][m.dc] == nullptr || grid[m.dr][m.dc]->getColor() != colour);
}

Bishop::Bishop(int row, int col, Colour colour)
    : Piece(row, col, colour, colour == WHITE ? "♗ " : "♝ ") {}

bool Bishop::isValidMove(Piece *grid[8][8], Move m)
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

Queen::Queen(int row, int col, Colour colour)
    : Piece(row, col, colour, colour == WHITE ? "♕ " : "♛ ") {}

bool Queen::isValidMove(Piece *grid[8][8], Move m)
{
    Rook rook(0, 0, colour);
    Bishop bishop(0, 0, colour);
    return rook.isValidMove(grid, m) || bishop.isValidMove(grid, m);
}

King::King(int row, int col, Colour colour)
    : Piece(row, col, colour, colour == WHITE ? "♔ " : "♚ ") {}

bool King::isValidMove(Piece *grid[8][8], Move m)
{
    int dr = abs(m.dr - m.sr);
    int dc = abs(m.dc - m.sc);

    if (dr > 1 || dc > 1)
    {
        return false;
    }

    return (grid[m.dr][m.dc] == nullptr || grid[m.dr][m.dc]->getColor() != colour);
}
