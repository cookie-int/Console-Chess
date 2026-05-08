#include <iostream>
#include <cmath>
#include <chrono>
#include <cstdlib>
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

// tracks remaining time for a player; supports timed and untimed modes
class Timer
{
private:
    int remainingSeconds;
    chrono::time_point<chrono::steady_clock> lastUpdate;
    bool running;

public:
    bool timed;

    Timer(int seconds = 300, bool isTimed = true)
    {
        remainingSeconds = seconds;
        running = false;
        timed = isTimed;
    }

    void start()
    {
        if (!running)
        {
            lastUpdate = chrono::steady_clock::now();
            running = true;
        }
    }

    void stop()
    {
        if (running)
        {
            auto now = chrono::steady_clock::now();
            int elapsed = chrono::duration_cast<chrono::seconds>(now - lastUpdate).count();
            remainingSeconds -= elapsed;
            running = false;
        }
    }

    int getRemaining() const
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
    string format() const
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
};

// player owns a name, colour, and their own timer
class Player
{
private:
    string name;
    Colour colour;
    Timer timer;

public:
    Player(string n, Colour c, int timeLimit, bool isTimed)
        : name(n), colour(c), timer(timeLimit, isTimed) {}

    string getName() const { return name; }
    Colour getColor() const { return colour; }
    Timer &getTimer() { return timer; }
};

class Game
{
private:
    Player white;
    Player black;
    Colour turn;

public:
    Game(string whiteName, string blackName, bool isTimed)
        : white(whiteName, WHITE, isTimed ? 600 : 0, isTimed),
          black(blackName, BLACK, isTimed ? 600 : 0, isTimed),
          turn(WHITE) {}

    Player &getCurrentPlayer()
    {
        return (turn == WHITE) ? white : black;
    }

    Player &getWhite() { return white; }
    Player &getBlack() { return black; }

    Colour getTurn() const { return turn; }

    void switchTurn()
    {
        turn = (turn == WHITE) ? BLACK : WHITE;
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
