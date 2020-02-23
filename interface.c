/******************************************************************************
 * File: interface.c                                                          *
 * Author: Anton R                                                            *
 * Created: 28/09/2019                                                        *
 * Modified: 20/10/2019                                                       *
 * Purpose: Defines functions for interacting with the user through the       *
 *          terminal console.                                                 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interface.h"
#include "game.h"
#include "fileIO.h"


/******************************************************************************
 * Static function prototypes:                                                *
 * These functions are for drawing purposes only and are not intended for use *
 * outside of interface.c                                                     *
 ******************************************************************************/
static void _draw_start( void );
static void _draw_end( void );
static void _col_set( int );
static void _col_reset( );


/******************************************************************************
 * Function: menuSelection                                                    *
 * Imports: none                                                              *
 * Exports: a menu selection which represents the next gamestate              *
 * Purpose: provide the user with a list of options that they can choose from *
 *          to navigate the game's interface                                  *
 ******************************************************************************/
GameState menuSelection( char *title, const char **menuItems, int lastItem )
{
    int i;
    GameState selection;
    char *prompt;

    /* print the title, 
     * then print each menu item with their corresponding number */
    printf( "\n%s:\n", title );
    for ( i = 0; i <= lastItem; i++ )
    {
        printf( ">> %d. %s\n", i + 1, menuItems[i] );
    }

    /* prompt user for a selection */
    prompt = "Please select an option: ";
    do {
        selection = (GameState) ( getInt( prompt ) - 1 );

        prompt = "Please enter a valid option: ";
    } while ( selection < NewGame || selection > Exit );

    return selection;
}



/******************************************************************************
 * Function: welcomeMessage                                                   *
 * Imports: none                                                              *
 * Exports: none                                                              *
 * Purpose: Displays a welcome message at the start of the game to give the   *
 *          user an idea of what the game is about                            *
 ******************************************************************************/
void welcomeMessage( void )
{
    clearScreen( );
    printf( "   ~~~~~  Welcome to M-N-K Tic-Tac-Toe!!  ~~~~~\n\n"
            "The rules of the game are:\n"
            "- The board is M cells wide and N cells high\n"
            "- Players take turns placing a tile on an empty space on "
            "the board\n"
            "- The first player to place K tiles in a row wins!\n"
            "- Tiles can be lined up vertically, horizontally, "
            "or diagonally\n\n");

    enterToContinue( );
}



/******************************************************************************
 * Function: drawGameBoard                                                    *
 * Imports: a pointer to a GameBoard struct                                   *
 * Exports: none                                                              *
 * Purpose: Displays the current board state to the user.                     *
 *          Uses box-drawing characters to draw the board (see interface.h)   *
 ******************************************************************************/
void drawGameBoard( GameBoard *board )
{
    int width, height;
    int i, j;

    width = board->settings->width;
    height = board->settings->height;

    printf( "Current Game Board:\n" );

    /* Print column numbers.
     * each number is in a right-aligned box 4 character wide so that they
     * can align with the board's columns */
    printf( " " );
    for ( i = 0; i < width; i++ )
    {
        printf( "%4d", i );
    }
    printf( "\n" );

    /* draw top frame */
    _draw_start( );
    printf( "  %c", BOX_TOP_LEFT );
    for ( i = 0; i < width - 1; i++ )
    {
        printf( "%c%c%c%c", BOX_HORIZONTAL, BOX_HORIZONTAL,
                           BOX_HORIZONTAL, BOX_TOP_T );
    }
    printf( "%c%c%c%c\n", BOX_HORIZONTAL, BOX_HORIZONTAL,
                         BOX_HORIZONTAL, BOX_TOP_RIGHT );

    /* draw rows */
    for ( i = 0; i < height - 1; i++ )
    {
        printf( "%2d%c", i, BOX_VERTICAL );

        for ( j = 0; j < width; j++ )
        {
            _draw_end( );
            printTile( board->tiles[i][j] );
            _draw_start( );
            printf( "%c", BOX_VERTICAL );
        }
        printf( "\n" );

        printf( "  %c", BOX_LEFT_T );
        for ( j = 0; j < width - 1; j++ )
        {
            printf( "%c%c%c%c", BOX_HORIZONTAL, BOX_HORIZONTAL,
                               BOX_HORIZONTAL, BOX_CROSS );
        }
        printf( "%c%c%c%c\n", BOX_HORIZONTAL, BOX_HORIZONTAL,
                             BOX_HORIZONTAL, BOX_RIGHT_T );
    }

    /* last row and bottom frame */
    i = height - 1;
    printf( "%2d%c", i, BOX_VERTICAL );
    for ( j = 0; j < width; j++ )
    {
        _draw_end( );
        printTile( board->tiles[i][j] );
        _draw_start( );
        printf( "%c", BOX_VERTICAL );
    }
    printf( "\n" );

    printf( "  %c", BOX_BOTTOM_LEFT );
    for ( i = 0; i < width - 1; i++ )
    {
        printf( "%c%c%c%c", BOX_HORIZONTAL, BOX_HORIZONTAL,
                           BOX_HORIZONTAL, BOX_BOTTOM_T );
    }
    printf( "%c%c%c%c", BOX_HORIZONTAL, BOX_HORIZONTAL,
                       BOX_HORIZONTAL, BOX_BOTTOM_RIGHT );
    _draw_end( );
    printf( "\n" );
}



/******************************************************************************
 * Function: printTile                                                        *
 * Imports: tile to print                                                     *
 * Exports: none                                                              *
 * Purpose: Prints out a text representation of a Tic-Tac-Toe tile.           *
 *          Pads the character with a space on either side for better         *
 *          readability and aesthetics.                                       *
 ******************************************************************************/
void printTile( TicTacTile tile )
{
    if ( tile == TicTacNone )
    {
        printf( "   " );
    }
    else if ( tile == TicTacX )
    {
        _col_set( COL_RED_FG );
        printf( " X " );
        _col_reset( );
    }
    else if ( tile == TicTacO )
    {
        _col_set( COL_GREEN_FG );
        printf( " O " );
        _col_reset( );
    }
}



/******************************************************************************
 * Function: getInt                                                           *
 * Imports: prompt string                                                     *
 * Exports: returns an integer value                                          *
 * Purpose: Prompts the user for an integer value                             *
 *          Validates the input and continues prompting until a valid number  *
 *          is entered.                                                       *
 ******************************************************************************/
int getInt( char *prompt )
{
    int value, scanResult;
    BOOL valid = TRUE;

    do
    {
        printf( "%s", prompt );
        scanResult = scanf( "%d", &value );
        valid = TRUE;

        /* make sure there are no characters left in the buffer */
        valid = clearBuffer( );

        if ( scanResult != 1 || !valid )
        {
            printErr( "Please enter a valid number" );
        }
    }
    while ( scanResult != 1 || !valid );

    return value;
}



/******************************************************************************
 * Function: getCoordinates                                                   *
 * Imports: prompt string                                                     *
 *          max values of X and Y                                             *
 * Exports: a Coordinates struct                                              *
 * Purpose: prompts the user for a pair of integers                           *
 *          Validates the input and continues prompting until two numbers     *
 *          in a valid range are entered.                                     *
 ******************************************************************************/
Coordinates getCoordinates( char *prompt, int maxX, int maxY )
{
    Coordinates coords;
    char *buffer, *delimPointer;
    int buffSize;
    BOOL valid;

    /* allocate enough memory for two numbers, two +/- signs, a comma,
     * a newline, and a null terminator. Everything else will be discarded */
    buffSize = MAX_DIGITS * 2 + 5;
    buffer = (char*) malloc( buffSize * sizeof(char) );

    do
    {
        /* get the coordinate string from the user */
        printf( "%s", prompt );
        fgets( buffer, buffSize, stdin );

        /* validate the coordinates and read the values into the struct */
        valid = validCoordString( buffer );
        if ( valid )
        {
            int i;
            char *temp;

            /* read the x and y values as base-10 integers */
            coords.x = strtol( buffer, &delimPointer, 10 );
            i = skipSpace( delimPointer );

            temp = delimPointer + i + 1;
            coords.y = strtol( temp, &delimPointer, 10 );
            /* the contents of delimPointer are not checked because the 
             * string is assumed to be valid */
        }

        /* if the buffer doesn't end with a newline, then there must be
         * characters left in stdin (leftover from fgets) */
        if ( buffer[strlen( buffer ) - 1] != '\n' )
        {
            /* if buffer is whitespace only,
             * the coordinates may still be valid */
            if ( !clearBuffer( ) )
            {
                valid = FALSE;
            }
        }

        if ( !valid )
        {
            printErr( "Please enter valid coordinates" );
        }
        else if ( coords.x >= maxX || coords.y >= maxY ||
                  coords.x < 0 || coords.y < 0 )
        {
            printErr( "Coordinates outside of valid range" );
        }
    }
    while ( !valid || coords.x >= maxX || coords.y >= maxY ||
        coords.x < 0 || coords.y < 0 );

    /* clean up */
    free( buffer );

    return coords;
}



/******************************************************************************
 * Function: clearBuffer                                                      *
 * Imports: none                                                              *
 * Returns: TRUE if there were only whitespace characters left in the buffer  *
 *          FALSE otherwise                                                   *
 * Exports: none                                                              *
 * Purpose: removes any characters left in the buffer for future input,       *
 *          and checks if there were any non-space characters left for        *
 *          validation                                                        *
 ******************************************************************************/
BOOL clearBuffer( void )
{
    BOOL valid = TRUE;
    char ch;

    do
    {
        ch = getchar();
        if ( !isWhiteSpace( ch ) )
        {
            valid = FALSE;
        }
    }
    while ( ch != '\n' );

    return valid;
}



/******************************************************************************
 * Function: isWhiteSpace                                                     *
 * Imports: ch (char)                                                         *
 * Exports: none                                                              *
 * Returns: TRUE if the imported character is whitespace,                     *
 *          FALSE otherwise                                                   *
 * Purpose: Checks is a given characters is a whitespace character.           *
 *          Characters considered whitespace are NEWLINE, TAB, and SPACE      *
 ******************************************************************************/
BOOL isWhiteSpace( char ch )
{
    BOOL whiteSpace = TRUE;

    if ( ch != '\n' && ch != '\t' && ch != ' ' )
    {
        whiteSpace = FALSE;
    }

    return whiteSpace;
}



/******************************************************************************
 * Function: validCoordString                                                 *
 * Imports: pointer to a string                                               *
 * Exports: none                                                              *
 * Returns: TRUE if the string is a valid set of coordinates                  *
 *          FALSE otherwise                                                   *
 * Purpose: Validates a string reppresenting coordinates on a game board.     *
 *          The expected format for the cooridinates is:                      *
 *                                                                            *
 *              [ws] 1-2 digits [ws] comma [ws] 1-2 digits [ws][\n]           *
 *                                                                            *
 *          where 'ws' represents an arbitrary amount of whitespace.          *
 ******************************************************************************/
BOOL validCoordString( char* string )
{
    BOOL valid = FALSE;
    int i;
    char *delimPointer;

    /* attempt to convert the string into a base-10 integer.
     * If the pointer difference between string and delimPointer is zero, 
     * then no digits were read and there is no number */
    strtol( string, &delimPointer, 10 );
    if ( delimPointer - string > 0 )
    {
        /* skip spaces and check if the next character is a comma */
        i = skipSpace( delimPointer );
        if ( delimPointer[i] == ',' )
        {
            /* attempt to read another integer from the string */
            char *temp = delimPointer + i + 1;
            strtol( temp, &delimPointer, 10 );
            if ( delimPointer - temp > 0 )
            {
                /* skip spaces again and make sure the string ends with a
                 * newline or a null terminator*/
                i = skipSpace( delimPointer );
                if ( delimPointer[i] == '\0' || delimPointer[i] == '\n' )
                {
                    valid = TRUE;
                }
            }
        }
    }

    return valid;
}



/******************************************************************************
 * Function: skipSpace                                                        *
 * Imports: pointer to a string                                               *
 * Exports: index inside the string                                           *
 * Purpose: This function scans the string until a non-whitespace character   *
 *          is found, and returns the index of that character.                *
 *          Newline characters are treated as non-whitespace as they are      *
 *          only expected at the end of the string.                           *
 ******************************************************************************/
int skipSpace( char* string )
{
    int i = 0;

    while ( string[i] == ' ' || string[i] == '\t' )
    {
        i++;
    }

    return i;
}



/******************************************************************************
 * Function: clearScreen                                                      *
 * Imports: none                                                              *
 * Exports: none                                                              *
 * Purpose: clears the terminals screen                                       *
 * Functionality: uses the ANSI escape code ESC[1J to clear the terminal      *
 *                and ESC[1;1H to move the cursor to top left of the screen   *
 *                                                                            *
 * Based on information from:                                                 *
 *      https://www.tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html                *
 *      (last accessed 26/10/2019)                                            *
 ******************************************************************************/
void clearScreen( void )
{
    printf( "\x1b[1J\x1b[1;1H" );
}



/******************************************************************************
 * Function: enterToContinue                                                  *
 * Imports: none                                                              *
 * Exports: none                                                              *
 * Purpose: Waits for the user to press the ENTER key                         *
 ******************************************************************************/
void enterToContinue( void )
{
    char ch;

    printf( "Press [ENTER] " );
    do
    {
        ch = getchar( );
    } while ( ch != '\n' );
}



/******************************************************************************
 * Function: printErr                                                         *
 * Imports: error strin to be displayed                                       *
 * Exports: none                                                              *
 * Purpose: Prints a visually highlighted error message to the user.          *
 *          Used to make it easier for the user to distinguish errors from    *
 *          normal program output.                                            *
 *                                                                            *
 * Based on information from:                                                 *
 *      http://ascii-table.com/ansi-escape-sequences.php                      *
 *      (last accessed 26/10/2019)                                            *
 ******************************************************************************/
void printErr( char *errMsg )
{
    printf( "%sERROR:%s %s\n", COL_ERROR, COL_RESET, errMsg );
}



/******************************************************************************
 * Function: _draw_start                                                      *
 * Imports: none                                                              *
 * Exports: none                                                              *
 * Purpose: uses an ANSI sequence to switch to the DEC special character set  *
 *                                                                            *
 * Based on information from:                                                 *
 *      https://vt100.net/docs/vt100-ug/table3-9.html                         *
 *      (last accessed 26/10/2019)                                            *
 ******************************************************************************/
static void _draw_start( void )
{
    printf( "%s", DRAW_START );
}



/******************************************************************************
 * Function: _draw_end                                                        *
 * Imports: none                                                              *
 * Exports: none                                                              *
 * Purpose: switches back to normal ASCII                                     *
 *                                                                            *
 * Based on information from:                                                 *
 *      https://vt100.net/docs/vt100-ug/table3-9.html                         *
 *      (last accessed 26/10/2019                                             *
 ******************************************************************************/
static void _draw_end( void )
{
    printf( "%s", DRAW_END );
}



/******************************************************************************
 * Function: _col_set                                                         *
 * Imports: ANSI integer colour code                                          *
 * Exports: none                                                              *
 * Purpose: sets the terminal output colour according to the imported value   *
 *                                                                            *
 * Based on information from:                                                 *
 *      http://ascii-table.com/ansi-escape-sequences.php                      *
 *      (last accessed 26/10/2019)                                            *
 ******************************************************************************/
static void _col_set( int col )
{
    printf( "\x1b[%dm", col );
}



/******************************************************************************
 * Function: _col_reset                                                       *
 * Imports: none                                                              *
 * Exports: none                                                              *
 * Purpose: resets the terminal output colour back to default                 *
 *                                                                            *
 * Based on information from:                                                 *
 *      http://ascii-table.com/ansi-escape-sequences.php                      *
 *      (last accessed 26/10/2019)                                            *
 ******************************************************************************/
static void _col_reset( )
{
    printf( "%s", COL_RESET );
}
