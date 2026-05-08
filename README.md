# Console Chess Game in C++

A fully functional console-based Chess game developed in C++ as a final project for the Object Oriented Programming course.

The project demonstrates core OOP concepts such as inheritance, polymorphism, abstraction, encapsulation, and dynamic binding while implementing the complete logic of a playable chess game.

---

# Features

- Complete Chess board implementation
- All standard Chess pieces
  - Pawn
  - Rook
  - Knight
  - Bishop
  - Queen
  - King
- Legal move validation
- Check detection
- Checkmate detection
- Turn-based gameplay
- Timed and untimed game modes
- Highlighting of legal moves
- Unicode Chess pieces
- Colored console board interface
- Player timer system
- Object Oriented design

---

# OOP Concepts Used

This project was designed using major Object Oriented Programming principles.

## Inheritance

All Chess pieces inherit from the base `Piece` class.

```cpp
class Pawn : public Piece
```

## Polymorphism

Virtual functions are used for move validation.

```cpp
virtual bool isValidMove(Piece *grid[8][8], Move m) = 0;
```

## Abstraction

The base `Piece` class defines common behavior while derived classes implement their own movement logic.

## Encapsulation

Class data members are protected/private and accessed using member functions.

## Dynamic Binding

Runtime polymorphism is used through virtual functions and base class pointers.

---

# Technologies Used

- C++
- Object Oriented Programming
- Windows Console API
- UTF-8 Unicode Characters

---

# Project Structure

This project follows a 3-file structure.

```text
project-folder/
│
├── main.cpp
├── chess.h
├── chess.cpp
└── README.md
```

## File Description

### `main.cpp`

Contains the main game loop and user interaction.

### `chess.h`

Contains:
- Class declarations
- Function prototypes
- Struct definitions
- Enum definitions

### `chess.cpp`

Contains:
- Class function definitions
- Game logic
- Board display functions
- Move validation logic

---

# How to Run

## Requirements

- Windows Operating System
- g++ compiler (MinGW recommended)

---

## Compile

Open terminal in the project folder and run:

```bash
g++ main.cpp chess.cpp -o chess
```

---

## Run

```bash
./chess
```

or on some systems:

```bash
chess.exe
```

---

# Game Instructions

## Selecting a Piece

Enter board coordinates such as:

```text
e2
```

## Moving a Piece

After selecting a piece, enter the destination coordinate.

Example:

```text
e4
```

## Cancel Selection

```text
0
```

---

# Game Modes

## Timed Mode

- 10 minutes per player
- Timer decreases during each player's turn
- A player loses if their timer reaches zero

## Untimed Mode

- No time limit
- Free gameplay

---

# Chess Rules Implemented

- Valid piece movement
- Capturing opponent pieces
- Check detection
- Checkmate detection
- Illegal move prevention
- Turn switching

---

# Console Interface

The game includes:
- Unicode Chess symbols
- Colored board squares
- Move highlighting
- Capture highlighting
- Player information panel
- Live timers

---

# Example Gameplay

```text
white select piece: e2
select destination: e4
```

---

# Learning Outcomes

This project helped demonstrate:
- Object Oriented Programming concepts
- Game logic implementation
- Console-based UI development
- Dynamic memory usage
- Class hierarchy design
- Problem solving and debugging

---

# Future Improvements

Possible future enhancements include:

- Castling
- En passant
- Pawn promotion
- Save/load game
- Undo move feature
- Multiplayer over network
- Graphical user interface

---

# Author

Developed as an Object Oriented Programming Final Project in C++.
