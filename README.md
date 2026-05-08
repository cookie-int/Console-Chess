# console chess

a fully functional two-player console chess game made in c++ for the object oriented programming lab course.

the game runs in the windows console and supports both timed and untimed gameplay with full move validation, check/checkmate detection, and highlighted legal moves.

## features

- full chess movement rules for all 6 piece types
- legal move validation
- check and checkmate detection
- timed mode with 10 minute countdown per player
- untimed gameplay option
- unicode chess pieces
- coloured board using windows console api
- legal move highlighting
- capture highlighting
- algebraic notation input like `e2` and `d4`

## oop concepts used

### encapsulation
data and functions are grouped inside classes like `piece`, `player`, `timer`, and `game`.

### inheritance
all chess pieces inherit from the abstract `piece` base class.

### polymorphism
each piece overrides `isValidMove()` differently and runtime polymorphism is used through `Piece*` pointers.

### composition
- `game` has two `player` objects
- `player` has a `timer`
- `queen` reuses rook and bishop movement logic internally
