#pragma once
#include <iostream>
#include <cmath>
#include <string>
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
    Piece(int row, int col, Colour colour, string symbol);

    string getSymbol();
    Colour getColor();

    virtual bool isValidMove(Piece *grid[8][8], Move m) = 0;

    virtual ~Piece() {}
};

// pawn moves forward 1, forward 2 from start row, or captures diagonally
// direction depends on colour: white goes up (-1), black goes down (+1)
class Pawn : public Piece
{
public:
    Pawn(int row, int col, Colour colour);

    int dir() const;

    bool isValidMove(Piece *grid[8][8], Move m) override;
};

// rook moves any number of squares along a row or column, path must be clear
class Rook : public Piece
{
public:
    Rook(int row, int col, Colour colour);

    bool isValidMove(Piece *grid[8][8], Move m) override;
};

// knight moves in an l-shape (2+1), jumps over pieces
class Knight : public Piece
{
public:
    Knight(int row, int col, Colour colour);

    bool isValidMove(Piece *grid[8][8], Move m) override;
};

// bishop moves diagonally any number of squares, path must be clear
class Bishop : public Piece
{
public:
    Bishop(int row, int col, Colour colour);

    bool isValidMove(Piece *grid[8][8], Move m) override;
};

// queen combines rook and bishop movement
class Queen : public Piece
{
public:
    Queen(int row, int col, Colour colour);

    bool isValidMove(Piece *grid[8][8], Move m) override;
};

// king moves exactly 1 square in any direction
class King : public Piece
{
public:
    King(int row, int col, Colour colour);

    bool isValidMove(Piece *grid[8][8], Move m) override;
};
