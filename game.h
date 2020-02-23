/******************************************************************************
 * File:---- game.h                                                           *
 * Author:-- Anton R                                                          *
 * Created:- 28/09/2019                                                       *
 * Modified: 20/10/2019                                                       *
 * Purpose:- Defines a number of gameplay-related datatypes for ease of       *
 *           game management.                                                 *
 *           Declares functions that control the flow of gameplay.            *
 ******************************************************************************/

#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include "linkedList.h"
#include "bool.h"


/******************************************************************************
 * MAX_DIMENSION: maximum allowed size of a game board dimension              *
 *                capped at 2 digits as settings this large will not ever     *
 *                be needed                                                   *
 ******************************************************************************/
#define MAX_DIMENSION 99
#define MAX_DIGITS 2



/******************************************************************************
 * MENU_ITEMS: an array of string representing the main menu items            *
 * Notes: this array is used for printing out the main menu with the correct  * 
 *        numbering regardless of how many game states are defined.           *
 *        The 'extern' keyword is used since initialising an array of items   *
 *        in a header file is bad practice and often leads to errors.         *
 *        The array is instead initialised in the corresponding .c fie        *
 ******************************************************************************/
extern const char *MENU_ITEMS[];



/******************************************************************************
 * TicTacTile: An enumeration of the possible tile states: X, O, or empty.    *
 *             Since it is an enum, new states can be added easily            *
 *             in the future, as long as TicTacTotal remains the last value.  *
 *             TicTacTotal is used to calculate the number of players.        *
 ******************************************************************************/
typedef enum
{
    TicTacNone,
    TicTacX,
    TicTacO,
    TicTacTotal
} TicTacTile;



/******************************************************************************
 * GameState: An enumeration of the possible game states.                     *
 *            Used to determine which interface to display.                   *
 ******************************************************************************/
typedef enum
{
    NewGame,
    ViewSettings,
    #ifdef Editor
    EditSettings,
    #endif
    ViewLog,
    #ifndef Secret
    SaveLog,
    #endif
    Exit
} GameState;



/******************************************************************************
 * Coordinates: A struct that holds the x and y values of a tile on the board *
 * Consist of:                                                                *
 *     x - horizontal coordinate                                              *
 *     y - vertical coordinate                                                *
 ******************************************************************************/
typedef struct
{
    int x;
    int y;
} Coordinates;



/******************************************************************************
 * Settings: A struct that holds the settings for a particular game.          *
 * Consists of:                                                               *
 *     width - the width of the game board                                    *
 *     height - the height of the game board                                  *
 *     matches - the number of identical tiles in a row required              *
 *               to win the game                                              *
 ******************************************************************************/
typedef struct
{
    int width;
    int height;
    int matches;
} Settings;



/******************************************************************************
 * GameBoard: A struct that holds the game board as well as the settings of   *
 *            that board.                                                     *
 * Consists of:                                                               *
 *     settings - a struct with settings for this particular board            *
 *     tiles - a 2d array of tile states                                      *
 ******************************************************************************/
typedef struct
{
    Settings *settings;
    TicTacTile **tiles;
} GameBoard;



/******************************************************************************
 * TurnLog: A struct that holds the information of a players turn.            *
 * Consists of:                                                               *
 *     turnNum - the turn on which this move was made                         *
 *     player - the numeric value of the player that made the turn            *
 *     location - the coordinates of where the tile was placed                *
 ******************************************************************************/
typedef struct
{
    int turnNum;
    TicTacTile player;
    Coordinates location;
} TurnLog;



/******************************************************************************
 * GameLog: A struct that holds the complete log of a TicTacToe game.
 * Consists of:
 *     settings - if Editor is enabled, each GameLog struct holds a copy of
 *                that game's settings
 *     turnList - a chronological linked list of TurnLog structs
 ******************************************************************************/
typedef struct
{
    #ifdef Editor
    Settings settings;
    #endif
    LinkedList *turnList;
} GameLog;



/* Function Prototypes */

void gameLoop( Settings* );
GameLog *playGame( Settings* );
void displaySettings( Settings* );
void editSettings( Settings* );
void displayLog( LinkedList*, Settings* );
void saveLog( LinkedList*, Settings* );

void boardInit( GameBoard*, Settings* );
BOOL placeTile( GameBoard*, TicTacTile, int, int );
BOOL checkWin( GameBoard*, TicTacTile, int, int );
int  checkDirection( GameBoard*, TicTacTile, int, int, int, int );
BOOL checkDraw( GameBoard* );
void changePlayer( TicTacTile* );
void newSetting( char*, int* );

void printGameLog( FILE*, void* );
void printTurnLog( FILE*, void* );

void deleteGameBoard( GameBoard* );
void deleteGameLog( void* );
void deleteTurnLog( void* );

#endif
