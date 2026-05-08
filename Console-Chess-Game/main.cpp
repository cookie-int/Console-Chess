#include <iostream>
#include <cstdlib>
#include <windows.h>
#include "pieces.h"
#include "game.h"
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    system("cls");

    cout << "  ╔══════════════════════╗" << endl;
    cout << "  ║         CHESS        ║" << endl;
    cout << "  ╚══════════════════════╝" << endl;

    string whiteName, blackName;
    int mode;

    cout << "Enter player 1 name (white): ";
    getline(cin, whiteName);

    cout << "Enter player 2 name (Black) ";
    getline(cin, blackName);

    cout << "╔════════════════════════════╗" << endl;
    cout << "║ 1. Timed (10 minutes each) ║" << endl;
    cout << "║ 2. Untimed                 ║" << endl;
    cout << "╚════════════════════════════╝" << endl;
    cout << "choice: ";
    cin >> mode;
    cin.ignore();

    bool isTimed = (mode == 1);

    Game game(whiteName, blackName, isTimed);
    game.getCurrentPlayer().getTimer().start();

    Piece *grid[8][8];
    initializeBoard(grid);

    Position selected = {-1, -1};
    string input;

    while (true)
    {
        system("cls");
        displayBoard(grid, selected, game.getTurn(), game);

        Colour turn = game.getTurn();

        if (isTimeOut(game))
        {
            cout << (turn == WHITE ? "black" : "white") << " wins on time!\n";
            break;
        }

        if (isCheckmate(grid, turn))
        {
            cout << (turn == WHITE ? "black" : "white") << " wins by checkmate!\n";
            break;
        }

        if (isKingInCheck(grid, turn))
        {
            cout << "check!\n";
        }

        if (selected.r == -1)
        {
            cout << game.getCurrentPlayer().getName() << " select piece (e.g. e2): ";
            getline(cin, input);

            if (input == "0")
            {
                continue;
            }
            if (input.length() < 2)
            {
                continue;
            }

            Position pos;
            pos.c = input[0] - 'a';
            pos.r = 8 - (input[1] - '0');

            if (grid[pos.r][pos.c] != nullptr && grid[pos.r][pos.c]->getColor() == turn)
            {
                selected = pos;
            }
        }
        else
        {
            cout << "select destination (0 to cancel): ";
            getline(cin, input);

            if (input == "0")
            {
                selected = {-1, -1};
                continue;
            }
            if (input.length() < 2)
            {
                continue;
            }

            Move m;
            m.sr = selected.r;
            m.sc = selected.c;
            m.dc = input[0] - 'a';
            m.dr = 8 - (input[1] - '0');

            Piece *p = grid[selected.r][selected.c];

            if (p == nullptr)
            {
                cout << "no piece selected!\n";
            }
            else if (p->getColor() != turn)
            {
                cout << "not your piece!\n";
            }
            else if (!p->isValidMove(grid, m))
            {
                cout << "invalid move!\n";
            }
            else if (!makeMoveAndTest(grid, m, turn))
            {
                cout << "illegal move: king would be in check!\n";
            }
            else
            {
                movePiece(grid, m, turn);
                selected = {-1, -1};
                game.getCurrentPlayer().getTimer().stop();
                game.switchTurn();
                game.getCurrentPlayer().getTimer().start();
            }

            Sleep(800);
        }
    }

    return 0;
}
