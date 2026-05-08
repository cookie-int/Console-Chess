#include "game.h"
#include "pieces.h"

Timer::Timer(int seconds, bool isTimed)
{
    remainingSeconds = seconds;
    running = false;
    timed = isTimed;
}

void Timer::start()
{
    if (!running)
    {
        lastUpdate = chrono::steady_clock::now();
        running = true;
    }
}

void Timer::stop()
{
    if (running)
    {
        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - lastUpdate).count();
        remainingSeconds -= elapsed;
        running = false;
    }
}

int Timer::getRemaining() const
{
    if (!timed)
    {
        return INT_MAX;
    }

    int sec = remainingSeconds;

    if (running)
    {
        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - lastUpdate).count();
        sec -= elapsed;
    }

    return sec;
}

// returns time as mm:ss string, or "--:--" for untimed
string Timer::format() const
{
    if (!timed)
    {
        return "--:--";
    }

    int sec = remainingSeconds;

    if (running)
    {
        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - lastUpdate).count();
        sec -= elapsed;
    }

    if (sec < 0)
    {
        sec = 0;
    }

    int m = sec / 60;
    int s = sec % 60;

    return to_string(m) + ":" + (s < 10 ? "0" : "") + to_string(s);
}

// player owns a name, colour, and their own timer
Player::Player(string n, Colour c, int timeLimit, bool isTimed)
    : name(n), colour(c), timer(timeLimit, isTimed) {}

string Player::getName() const { return name; }
Colour Player::getColor() const { return colour; }
Timer &Player::getTimer() { return timer; }

Game::Game(string whiteName, string blackName, bool isTimed)
    : white(whiteName, WHITE, isTimed ? 600 : 0, isTimed),
      black(blackName, BLACK, isTimed ? 600 : 0, isTimed),
      turn(WHITE) {}

Player &Game::getCurrentPlayer()
{
    return (turn == WHITE) ? white : black;
}

Player &Game::getWhite() { return white; }
Player &Game::getBlack() { return black; }

Colour Game::getTurn() const { return turn; }

void Game::switchTurn()
{
    turn = (turn == WHITE) ? BLACK : WHITE;
}

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

Position findKing(Piece *grid[8][8], Colour kingColor)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (grid[i][j] != nullptr && grid[i][j]->getColor() == kingColor)
            {
                if (dynamic_cast<King *>(grid[i][j]))
                {
                    return {i, j};
                }
            }
        }
    }

    return {-1, -1};
}

bool isKingInCheck(Piece *grid[8][8], Colour kingColor)
{
    Position king = findKing(grid, kingColor);

    if (king.r == -1)
    {
        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Piece *p = grid[i][j];

            if (p == nullptr || p->getColor() == kingColor)
            {
                continue;
            }

            Move testMove = {i, j, king.r, king.c};

            if (p->isValidMove(grid, testMove))
            {
                return true;
            }
        }
    }

    return false;
}

bool makeMoveAndTest(Piece *grid[8][8], Move m, Colour turn)
{
    Piece *captured = grid[m.dr][m.dc];
    Piece *moving = grid[m.sr][m.sc];

    grid[m.dr][m.dc] = moving;
    grid[m.sr][m.sc] = nullptr;

    bool inCheck = isKingInCheck(grid, turn);

    grid[m.sr][m.sc] = moving;
    grid[m.dr][m.dc] = captured;

    return !inCheck;
}

bool isLegalMove(Piece *grid[8][8], Move m, Colour turn)
{
    Piece *p = grid[m.sr][m.sc];

    if (p == nullptr)
    {
        return false;
    }
    if (p->getColor() != turn)
    {
        return false;
    }
    if (!p->isValidMove(grid, m))
    {
        return false;
    }
    if (!makeMoveAndTest(grid, m, turn))
    {
        return false;
    }

    return true;
}

void movePiece(Piece *grid[8][8], Move m, Colour currentTurn)
{
    Piece *p = grid[m.sr][m.sc];

    if (p == nullptr)
    {
        cout << "no piece here!\n";
        return;
    }
    if (p->getColor() != currentTurn)
    {
        cout << "not your piece!\n";
        return;
    }
    if (!p->isValidMove(grid, m))
    {
        cout << "invalid move!\n";
        return;
    }
    if (!makeMoveAndTest(grid, m, currentTurn))
    {
        cout << "illegal move: king would be in check!\n";
        return;
    }

    grid[m.dr][m.dc] = p;
    grid[m.sr][m.sc] = nullptr;
}

bool hasAnyLegalMove(Piece *grid[8][8], Colour turn)
{
    for (int sr = 0; sr < 8; sr++)
    {
        for (int sc = 0; sc < 8; sc++)
        {
            Piece *p = grid[sr][sc];

            if (p == nullptr || p->getColor() != turn)
            {
                continue;
            }

            for (int dr = 0; dr < 8; dr++)
            {
                for (int dc = 0; dc < 8; dc++)
                {
                    Move m{sr, sc, dr, dc};

                    if (!p->isValidMove(grid, m))
                    {
                        continue;
                    }

                    if (makeMoveAndTest(grid, m, turn))
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool isCheckmate(Piece *grid[8][8], Colour turn)
{
    if (!isKingInCheck(grid, turn))
    {
        return false;
    }

    if (hasAnyLegalMove(grid, turn))
    {
        return false;
    }

    return true;
}

bool isTimeOut(Game &game)
{
    Player &current = game.getCurrentPlayer();
    if (!current.getTimer().timed)
    {
        return false;
    }
    return current.getTimer().getRemaining() <= 0;
}

// board now has a unicode border, player info panel on the right,
// and highlights legal moves for the selected piece
void displayBoard(Piece *grid[8][8], Position selected, Colour turn, Game &game)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Player &white = game.getWhite();
    Player &black = game.getBlack();

    cout << "╔═════════════════════════════════════╗" << endl;
    cout << "║                 CHESS               ║" << endl;
    cout << "╚═════════════════════════════════════╝" << endl;

    for (int i = 0; i < 8; i++)
    {
        cout << "║ " << 8 - i << "  ";

        for (int j = 0; j < 8; j++)
        {
            bool isLight = (i + j) % 2 == 0;
            bool highlight = false;
            bool capture = false;

            if (selected.r != -1)
            {
                Move m{selected.r, selected.c, i, j};

                if (isLegalMove(grid, m, turn))
                {
                    highlight = true;
                    if (grid[i][j] != nullptr)
                    {
                        capture = true;
                    }
                }
            }

            if (capture)
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_INTENSITY);
            }
            else if (highlight)
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN);
            }
            else if (isLight)
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
            }
            else
            {
                SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY);
            }

            if (grid[i][j] == nullptr)
            {
                cout << "    ";
            }
            else
            {
                cout << " " << grid[i][j]->getSymbol() << " ";
            }
        }

        SetConsoleTextAttribute(hConsole, 7);

        cout << " ║ ";

        if (i == 1)
        {
            cout << "Black: " << black.getName() << " [" << black.getTimer().format() << "]";
        }
        else if (i == 6)
        {
            cout << "White: " << white.getName() << " [" << white.getTimer().format() << "]";
        }

        cout << endl;
    }

    cout << "║     a   b   c   d   e   f   g   h   ║" << endl;
    cout << "╚═════════════════════════════════════╝" << endl;
}
