#pragma once
#include <iostream>
#include <chrono>
#include <climits>
#include <string>
#include <windows.h>
#include "pieces.h"
using namespace std;

// tracks remaining time for a player; supports timed and untimed modes
class Timer
{
private:
    int remainingSeconds;
    chrono::time_point<chrono::steady_clock> lastUpdate;
    bool running;

public:
    bool timed;

    Timer(int seconds = 300, bool isTimed = true);

    void start();
    void stop();
    int getRemaining() const;

    // returns time as mm:ss string, or "--:--" for untimed
    string format() const;
};

// player owns a name, colour, and their own timer
class Player
{
private:
    string name;
    Colour colour;
    Timer timer;

public:
    Player(string n, Colour c, int timeLimit, bool isTimed);

    string getName() const;
    Colour getColor() const;
    Timer &getTimer();
};

class Game
{
private:
    Player white;
    Player black;
    Colour turn;

public:
    Game(string whiteName, string blackName, bool isTimed);

    Player &getCurrentPlayer();
    Player &getWhite();
    Player &getBlack();
    Colour getTurn() const;
    void switchTurn();
};

// board/logic free functions
void initializeBoard(Piece *grid[8][8]);
Position findKing(Piece *grid[8][8], Colour kingColor);
bool isKingInCheck(Piece *grid[8][8], Colour kingColor);
bool makeMoveAndTest(Piece *grid[8][8], Move m, Colour turn);
bool isLegalMove(Piece *grid[8][8], Move m, Colour turn);
void movePiece(Piece *grid[8][8], Move m, Colour currentTurn);
bool hasAnyLegalMove(Piece *grid[8][8], Colour turn);
bool isCheckmate(Piece *grid[8][8], Colour turn);
bool isTimeOut(Game &game);

// board now has a unicode border, player info panel on the right,
// and highlights legal moves for the selected piece
void displayBoard(Piece *grid[8][8], Position selected, Colour turn, Game &game);
