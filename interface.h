/******************************************************************************
 * File: interface.h                                                          *
 * Author: Anton R                                                            *
 * Created: 28/09/2019                                                        *
 * Modified: 26/10/2019                                                       *
 * Purpose: Defines a number of constants based on the ANSI standard escape   *
 *          sequences. These constants are used for changing the output       *
 *          colour for displaying errors and game tiles. They are also used   *
 *          to enable rendering box-drawing characters for the game board     *
 *          Declares functions that handle user interface interactions.       *
 ******************************************************************************/

#ifndef INTERFACE_H
#define INTERFACE_H

#include "game.h"

/* maximum length of an error message */
#define MAX_ERR_STRING 50


/******************************************************************************
 * ANSI text colour sequences                                                 *
 * Based on information from:                                                 *
 *      http://ascii-table.com/ansi-escape-sequences.php                      *
 *      (Last accessed 20/10/2019)                                            *
 ******************************************************************************/
#define COL_RESET    "\x1b[0m"
#define COL_ERROR    "\x1b[97;41m"

#define COL_RED_FG   31
#define COL_GREEN_FG 32



/******************************************************************************
 * ANSI escape secuences to start using the DEC special graphics character set*
 * Based on information from:                                                 *
 *      http://ascii-table.com/ansi-escape-sequences-vt-100.php               *
 *      (Last accessed 21/10/2019)                                            *
 ******************************************************************************/
#define DRAW_START "\x1b(0"
#define DRAW_END   "\x1b(B"



/******************************************************************************
 * DEC special graphics box-drawing characters                                *
 * Based on information from:                                                 *
 *      https://vt100.net/docs/vt100-ug/table3-9.html                         *
 *      (Last accessed 20/10/2019)                                            *
 ******************************************************************************/
#define BOX_BOTTOM_RIGHT 0x6a
#define BOX_TOP_RIGHT    0x6b
#define BOX_TOP_LEFT     0x6c
#define BOX_BOTTOM_LEFT  0x6d
#define BOX_CROSS        0x6e
#define BOX_HORIZONTAL   0x71
#define BOX_LEFT_T       0x74
#define BOX_RIGHT_T      0x75
#define BOX_BOTTOM_T     0x76
#define BOX_TOP_T        0x77
#define BOX_VERTICAL     0x78



/******************************************************************************
 * Function Prototypes                                                        *
 ******************************************************************************/
GameState menuSelection( char*, const char**, int );
void welcomeMessage( void );
void drawGameBoard( GameBoard* );
void printTile( TicTacTile );

void clearScreen( void );
void enterToContinue( void );
void printErr( char* );

int  getInt( char* );
Coordinates getCoordinates( char*, int, int );
BOOL clearBuffer( void );
BOOL isWhiteSpace( char );
BOOL validCoordString( char* );
int skipSpace( char* );

#endif
