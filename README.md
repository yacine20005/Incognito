# Incognito: The Game of Espionage

Welcome to Incognito, a thrilling two-player strategy board game of hidden identities, clever tactics, and secret missions. Unmask your opponent's spy before they find yours!

This project was developed by **Yacine Hamadouche** and **Liam Gavau-Pelissier** during their second year of study at Gustave Eiffel University.

---

## Game Overview

Incognito is a turn-based game played on a 5x5 grid. Each player commands an army of pieces, with one secretly designated as their **Spy**. The other pieces are **Knights**. Your opponent cannot see which of your pieces is the Spy, and you do not know which of theirs is the Spy.

### The Objective

There are two ways to win the game:
1.  **Capture the Spy:** Correctly identify and capture your opponent's Spy.
2.  **Infiltrate the Castle:** Successfully move your own Spy into the opponent's castle.

---

## How to Play

### The Board

The game is played on a square board. Each player has a castle in opposite corners. The board is initially populated with Black and White pieces protecting their respective castles.

```
  |b|b| | |   (b = White Knight, B = White Castle)
-- - - - - --
  |b| | | |
-- - - - - --
  | | |n| |   (n = Black Knight, N = Black Castle)
-- - - - - --
  | | | |n|
-- - - - - --
  | | |n|n|N|
```
*(Representation of a possible starting position)*

### Your Pieces

*   **Knights:** The bulk of your army. They are used to move, block, and attack.
*   **The Spy:** Your most important piece. It looks identical to a Knight to your opponent. You must use it to infiltrate the enemy castle or protect it from being discovered.

At the start of the game, one of your pieces is randomly and secretly chosen to be the Spy.

### Your Turn

On your turn, you can perform **one** of the following two actions:

**1. Move a Piece**
   - You can move any one of your pieces (Knight or Spy).
   - Pieces move like a queen in chess: any number of empty squares in a straight line, horizontally, vertically, or diagonally.
   - You **cannot** jump over any other piece, friendly or hostile.
   - You cannot move into your own castle.

**2. Interrogate an Enemy Piece**
   - This action is only possible when one of your pieces is directly adjacent (horizontally or vertically) to an enemy piece.
   - You choose one of your adjacent pieces to perform the interrogation.
   - **If the interrogated piece is the enemy Spy:** You have discovered their secret agent! You win the game.
   - **If the interrogated piece is an enemy Knight:** The interrogation fails. Your piece that performed the interrogation is removed from the game.
   - **If the piece removed was your Spy:** You have lost your Spy! You lose the game.

---

## Features

*   **Classic Turn-Based Strategy:** Engage in a battle of wits and deduction.
*   **ASCII Game Interface:** A clean, classic terminal-based display for playing the game.
*   **Save & Load Games:** Pause a game and resume it later. You can save your progress at any point and load it back from the command line.
*   **Debug Mode:** A special mode for developers (or cheaters!) that reveals the locations of the Spies on the board.

---

## Getting Started

### Compilation

To compile the game, use a C compiler like `gcc`. Navigate to the project directory and run the following command:

```sh
gcc -o incognito plateau.c
```

### Running the Game

To start a new game, simply execute the compiled file:

```sh
./incognito
```

### Command-Line Options

You can run the game with the following options:

*   `-s <filename>`: **Save** the current game state to a file. If no filename is provided during gameplay, it will save to `sauvegarde.inco`.
    ```sh
    # The game will automatically save to my_game.inco upon quitting via the 's' command
    ./incognito -s my_game.inco
    ```

*   `-c <filename>`: **Load** (or continue) a game from a previously saved file.
    ```sh
    ./incognito -c my_game.inco
    ```

*   `-d`: **Debug Mode**. Starts the game with the locations of both spies visible.
    ```sh
    ./incognito -d
    ```

You can also combine options:
```sh
# Load a game in debug mode and set a new save file
./incognito -c my_game.inco -s my_new_save.inco -d
```
