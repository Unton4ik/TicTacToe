# TicTacToe

This was my final assignment for Unix and C Programming (UCP). The task was to design and code (in C89) an M-N-K tic-tac-toe game.

The game was required to:
- Interact with the user via a terminal-based menu
- Extract the game settings from a file
- Play a game of tic-tac-toe of a board size of M x N
- Log all moves that have been executed
- Save the logs to a file

## Code Design
The game code is written in the C89 programming language and adheres to Curtin University's C/C++ coding standard. Each file has a header comment explaining its general purpose, and each constant, function and type definition is preceded by a detailed comment explaining its purpose and implementation.

## Input Files
The game program accepts one command-line argument that is the name of the setting file, for example:
 
`./TicTacToe setting.txt`

The settings file is expected to contain 3 values:
- M - the width of the board
- N - the height of the board
- K - the number of matching tiles in a row to win

A valid settings file would look as follows:
<pre><code>M=5
N=4
K=3</code></pre>

The settings are case-insensitive and can appear in any order. If the settings file contains any invalid, duplicate or missing values, the game will display an error to the user, notifying them of the first invalid value, and then exit safely.

## User Interface
The game has a menu that provides the user with the ability to:
1. Start a new game
2. View the game settings
3. Edit the game settings
4. View the currents logs
5. Save the logs to a file
6. Exit the application

The menu only requires an integer input to select the options

## Gameplay
The game play like regular tic-tac-toe, with each player taking turns placing a tile on an N x M board until someone gets K tiles in a line or no more tiles can be placed (in which case the game ends in a draw).

When playing the game, the board is displayed graphically in the terminal, using numbers, letters, and box-drawing characters. Each turn, the player is prompted for the coordinates of the tile they wish to place. If the tile is invalid or already occupied, the player is asked to enter the coordinates again until a valid set is entered.

Example game board after a player has won:
<pre><code>     0   1   2   3   4
   ┌───┬───┬───┬───┬───┐
 0 │ X │ O │   │   │   │
   ├───┼───┼───┼───┼───┤
 1 │   │ O │   │   │   │
   ├───┼───┼───┼───┼───┤
 2 │   │ O │ X │   │   │
   ├───┼───┼───┼───┼───┤
 3 │   │   │   │ X │   │
   └───┴───┴───┴───┴───┘
Player 2 has won!</code></pre>

## Logging
The game keeps a log of all actions performed by players during a game. Each player's turn is added to a **generic** linked list that can be printed or saved later. The logs are displayed in the following format:
<pre><code>GAME 1

SETTINGS:
  M: 5
  N: 4
  K: 3
  
  Turn: 1
  Player: X
  Location: 0,0
  
  Turn: 2
  Player: O
  Location: 1,1
  
  ...
  
GAME 2

SETTINGS:
  ...</code></pre>

When the logs are saved to a file, the name of the log file is in the format `MNK_<M>-<N>-<K>_<HOUR>-<MIN>_<DAY>-<MONTH>.log`, where `M`, `N` and `K` are the game settings, and the time is in the 24-hour format. An example log file would look like "MNK_3-4-2_18-20_09-07.log"

## Conditional Compilation
The proram's Makefile supports two compilation flags that can be used to compile the game with different functionality. These flags are:

- **Secret** - when the program is compiled with this flag, the user loses the ability to save logs to a file
- **Editor** - this flag lets the user edit the values of M, N and K at run time

Conditional compilation is achieved by using preprocessor macros.


###### Description adapted from the UNIX and C Programming final assignment specification
