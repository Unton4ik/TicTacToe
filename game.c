/******************************************************************************
 * File:---- game.c                                                           *
 * Author:-- Anton R                                                          *
 * Created:- 28/09/2019                                                       *
 * Modified: 20/10/2019                                                       *
 * Purpose:- Defines a set of functions that control the gameplay
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "interface.h"
#include "fileIO.h"
#include "bool.h"


/******************************************************************************
 * MENU_ITEMS: defines the array of menu items that was declared in the       *
 *             game.h header file.                                            *
 ******************************************************************************/
const char* MENU_ITEMS[] =
{
    "New Game",
    "View Settings",
    #ifdef Editor
    "Edit Settings",
    #endif
    "View Game Log",
    #ifndef Secret
    "Save Game Log",
    #endif
    "Exit"
};



/******************************************************************************
 * Function: gameLoop                                                         *
 * Imports: pointer to a GameSettings struct                                  *
 * Exports: none                                                              *
 * Purpose: The main gameplay function, determines the next gamestate based   *
 *          on user input in the main menu.                                   *
 *          Prompts the user for a choice based on the pre-defined set of     *
 *          menu items and their enumerated values. Keeps prompting until     *
 *          the user chooses the Exit option.                                 *
 ******************************************************************************/
void gameLoop( Settings *gameSettings )
{
    GameState gameState;
    LinkedList *gameLogList;

    gameLogList = createList( );

    /* get a new gamestate from the user and keep running until
	 * the user chooses to exit */
	do
	{
	    GameLog *newLog;
	
	    gameState = menuSelection( "MAIN MENU", MENU_ITEMS, (int) Exit );
	
	    /* choose next interface based on the current gamestate */
	    switch ( gameState )
	    {
	        case NewGame:
	            newLog = playGame( gameSettings );
	            listInsertLast( gameLogList, (void*) newLog );
	            enterToContinue( );
	            break;
	
	        #ifdef Editor
	        case EditSettings:
	            editSettings( gameSettings );
	            break;
	        #endif
	
	        case ViewSettings:
	            displaySettings( gameSettings );
	            break;
	
	        case ViewLog:
	            displayLog( gameLogList, gameSettings );
	            break;
	
	        #ifndef Secret
	        case SaveLog:
	            saveLog( gameLogList, gameSettings );
	            break;
	        #endif
	
	        case Exit:
	            printf( "Goodbye\n" );
	            break;
	
	        default:
	            printf( "Invalid option\n" );
	    }	
	}
	while ( gameState != Exit );
    
    /* clean up */
    freeList( gameLogList, &deleteGameLog );
}



/******************************************************************************
 * Function: playGame                                                         *
 * Imports: pointer to current game settings                                  *
 * Exports: pointer to a new game log                                         *
 * Purpose: This function starts a new game and plays it turn by turn until   *
 *          one of the players wins or all tiles are filled in an unwinnable  *
 *          way. Every turn is logged and saved into a GameLog struct, which  *
 *          is then returned to the calling function.                         *
 ******************************************************************************/
GameLog *playGame( Settings *settings )
{
    GameLog *gameLog;
    GameBoard *gameBoard;
    TicTacTile currentPlayer;
    int turnNum;
    BOOL finished;

    /* Initialize all gameplay variables */
    currentPlayer = TicTacX;
    gameLog = (GameLog*) malloc( sizeof(GameLog) );
    #ifdef Editor
    gameLog->settings = *settings;
    #endif
    gameLog->turnList = createList( );
    gameBoard = (GameBoard*) malloc( sizeof(GameBoard) );
    boardInit( gameBoard, settings );
    finished = FALSE;

    /* draw the game board and start playing */
    turnNum = 1;
    clearScreen( );
    drawGameBoard( gameBoard );
    while ( !finished )
    {
        TurnLog *log;
        Coordinates newTile;

        /* get a new tile from the player and place it on the board */
        printf( "Player %d's turn\n", ( int ) currentPlayer );
        newTile = getCoordinates( "Place a tile (x,y): ",
            settings->width, settings->height );

        if ( placeTile( gameBoard, currentPlayer, newTile.x, newTile.y ) )
        {
            log = (TurnLog*) malloc( sizeof(TurnLog) );
            log->turnNum = turnNum;
            log->player = currentPlayer;
            log->location = newTile;
            listInsertLast( gameLog->turnList, (void*) log );

            clearScreen( );
            drawGameBoard( gameBoard );
            
            /* check if the player has won, if not check for a draw */
            if ( checkWin( gameBoard, currentPlayer, newTile.x, newTile.y ) )
            {
                finished = TRUE;
                printf( "Player %d has won!\n", currentPlayer );
            }
            else if ( checkDraw( gameBoard ) )
            {
                finished = TRUE;
                printf( "All tiles are taken, this is a draw!\n" );
            }
            changePlayer( &currentPlayer );
            turnNum++;
        }
    }

    /* clean up */
    deleteGameBoard( gameBoard );

    return gameLog;
}



/******************************************************************************
 * Function: editSettings                                                     *
 * Imports: pointer to game settings struct                                   *
 * Returns: none                                                              *
 * Exports: changes all three values in the settings struct                   *
 * Purpose: Prompts the user for new values of the game settings.             *
 *          If the value of K is too large, notifies the user and prompts for *
 *          all three settings again.                                         *
 ******************************************************************************/
void editSettings( Settings *settings )
{
    displaySettings( settings );

    printf( "!! ALL SETTINGS MUST BE BETWEEN 1 AND 99 !!\n" );
    do
    {
        printf("\n");
        newSetting( "M (width)", &settings->width );
        newSetting( "N (height)", &settings->height );
        newSetting( "K (tiles in a row)", &settings->matches );

        if ( settings->matches > settings->height ||
            settings->matches > settings->width )
        {
            printErr( "The value of K cannot be larger than M or N" );
        }
    } while ( settings->matches > settings->height ||
        settings->matches > settings->width );
}



/******************************************************************************
 * Function: newSetting                                                       *
 * Imports: name of the settings to display                                   *
 *          pointer to the setting to change                                  *
 * Exports: new value of the setting                                          *
 * Purpose: prompts the user for a new value of a given setting until a value *
 *          in a valid range is given.                                        *
 ******************************************************************************/
void newSetting( char *setting, int *value )
{
    do
    {
        printf( "Enter new value of %s: ", setting );
        *value = getInt( "" );

        if ( *value <= 0 || *value > MAX_DIMENSION )
        {
            printErr( "Setting outside of valid range" );
        }
    } while ( *value <= 0 || *value > MAX_DIMENSION );
}



/******************************************************************************
 * Function: displaySettings                                                  *
 * Imports: pointer to a game settings struct                                 *
 * Exports: none                                                              *
 * Purpose: prints the current game settings to the user                      *
 ******************************************************************************/
void displaySettings( Settings *settings )
{
    printf( "\nThe game's settings are:\n\n"
           "  Board size: %dx%d\n"
           "  Win condition: %d tiles in a row\n\n",
           settings->width, settings->height, settings->matches );
}



/******************************************************************************
 * Function: displayLog                                                       *
 * Imports: pointer to a list of game logs                                    *
 *          pointer to the current settings struct                            *
 * Exports: none                                                              *
 * Purpose: prints the contents of the game log to the user using the         *
 *          printList function.                                               *
 *          If Editor is enabled, prints the settings of each individual game,*
 *          otherwise prints the current settings at the start                *
 ******************************************************************************/
void displayLog( LinkedList *logList, Settings *settings )
{
    int i;

    printf( "\n" );

    /* print settings first if editor is disabled */
    #ifndef Editor
    printf( "SETTINGS:\n"
           "  M: %d\n"
           "  N: %d\n"
           "  K: %d\n\n",
           settings->width, settings->height, settings->matches );
    #endif

    /* print each game log one by one */
    for ( i = 0; i < logList->size; i++ )
    {
        printf( "##################\n"
                "###   GAME %2d  ###\n"
                "##################\n", i + 1 );
        printElement( stdout, logList, i, &printGameLog );
    }

    printf( "\n" );
}



/******************************************************************************
 * Function: saveLog                                                          *
 * Imports: pointer to a list of game logs                                    *
 *          pointer to the current settings struct                            *
 * Exports: none                                                              *
 * Purpose: writes the contents of the game log into an external file which   *
 *          is named based on the current date, time and game settings.       *
 *          Format is identical to the displayLog function.                   *
 ******************************************************************************/
void saveLog( LinkedList *logList, Settings *settings)
{
    int i;
    char *fileName;
    FILE *outFile;

    fileName = (char*) malloc( LOG_NAME_LEN * sizeof(char) );
    outFile = createLogFile( settings, fileName );

    /* virtually same code as displayLog */
    #ifndef Editor
    fprintf( outFile, "SETTINGS:\n"
                      "  M: %d\n"
                      "  N: %d\n"
                      "  K: %d\n\n",
                      settings->width, settings->height, settings->matches );
    #endif

    for ( i = 0; i < logList->size; i++ )
    {
        fprintf( outFile, "##################\n"
                          "###   GAME %2d  ###\n"
                          "##################\n", i + 1 );
        printElement( outFile, logList, i, &printGameLog );
    }

    if ( !ferror( outFile ) )
    {
        printf( "\nGame logs have been saved to %s\n\n", fileName );
    }
    else
    {
        printErr( "Failed to write the logs to the output file\n" );
    }

    /* clean up */
    free(fileName);
    fclose(outFile);
}



/******************************************************************************
 * Function: boardInit                                                        *
 * Imports: pointer to a GameBoard struct                                     *
 *          pointer to a Settings struct                                      *
 * Exports: sets the values of all the board's fields                         *
 * Purpose: allocates memory for a new gameboard and sets the values of all   *
 *          cells to empty.                                                   *
 ******************************************************************************/
void boardInit( GameBoard *board, Settings *settings )
{
    int i, j;

    board->settings = settings;

    /* allocate memory for each row */
    board->tiles = (TicTacTile**) malloc(
        board->settings->height * sizeof( TicTacTile* ) );

    /* allocate memory for each column and set all values to empty tile */
    for ( i = 0; i < board->settings->height; i++ )
    {
        board->tiles[i] = (TicTacTile*) malloc(
            board->settings->width * sizeof( TicTacTile ) );

        for ( j = 0; j < board->settings->width; j++ )
        {
            board->tiles[i][j] = TicTacNone;
        }
    }
}



/******************************************************************************
 * Function: placeTile                                                        *
 * Imports: pointer to a GameBoard struct                                     *
 *          tile variable representing the current player                     *
 *          x and y coordinates of the new tile                               *
 * Returns: TRUE if the tile was placed on the board                          *
 *          FALSE if the could not be placed                                  *
 * Exports: none                                                              *
 * Purpose: attempts to place a new tile onto the game board.                 *
 *          If the spot is already taken, notifies the user and returns FALSE.*
 ******************************************************************************/
BOOL placeTile( GameBoard *board, TicTacTile tile, int x, int y )
{
    BOOL valid;
    valid = FALSE;

    /* ensure the tile is in a valid range */
    if ( x >= 0 && x < board->settings->width &&
        y >= 0 && y < board->settings->height )
    {
        /* if there is no other tile at the coordinates, place it */
        if ( board->tiles[y][x] == TicTacNone )
        {
            board->tiles[y][x] = tile;
            valid = TRUE;
        }
        else
        {
            printErr( "These coordinates are already taken!" );
        }
    }

    return valid;
}



/******************************************************************************
* Function: checkWin                                                          *
* Imports: pointer to a GameBoard struct                                      *
*          tile representing the current player                               *
*          x and y coordinates of the newest tile                             *
* Returns: TRUE if the current player has won the game                        *
*          FALSE otherwise                                                    *
* Exports: none                                                               *
* Purpose: This function checks if the current player has won the game by     *
*          scanning the tiles in 8 direcions from the tile they had just      *
*          placed.                                                            *
*          It validates the win by counting the number of relevant tiles in a *
*          row in each main direction (horizontal, vertical, and diagonals).  *
*          counting stops once an unwanted tile is found.                     *
*          Directions to be checked are as follows:                           *
*               (1, 0) - horizontal                                           *
*               (0, 1) - vertical                                             *
*               (1, 1) - desceding diagonal                                   *
*               (1,-1) - rising diagonal                                      *
******************************************************************************/
BOOL checkWin( GameBoard *board, TicTacTile tile, int x, int y )
{
    BOOL won = FALSE;
    int count;

    /* check column */
    count = checkDirection( board, tile, x, y, 1, 0 );
    if ( count >= board->settings->matches )
    {
        won = TRUE;
    }

    /* check row */
    if ( !won )
    {
        count = checkDirection( board, tile, x, y, 0, 1 );
        if ( count >= board->settings->matches )
        {
            won = TRUE;
        }
    }

    /* check diagonal (top-down) */
    if ( !won )
    {
        count = checkDirection( board, tile, x, y, 1, 1 );
        if ( count >= board->settings->matches )
        {
            won = TRUE;
        }
    }

    /* check diagonal (bottom-up) */
    if ( !won )
    {
        count = checkDirection( board, tile, x, y, 1, -1 );
        if ( count >= board->settings->matches )
        {
            won = TRUE;
        }
    }

    return won;
}



/******************************************************************************
 * Function: checkDirection                                                   *
 * Imports: pointer to a game board                                           *
 *          tile representing a current player                                *
 *          x and y coordinates of the new tile                               *
 *          x and y increments for the scanner                                *
 * Returns: number of relevant tiles in a row in the specified direction      *
 * Purpose: counts the number of tiles along a straight line.                 *
 *          Starts at the specified point and moves along a straight line     *
 *          until aninvalid tile, then returns to the staring point and       *
 *          moves in the opposite direction.                                  *
 ******************************************************************************/
int checkDirection( GameBoard *board, TicTacTile tile, int x, int y,
    int xStep, int yStep )
{
    int counter, i, j;
    TicTacTile currentTile = TicTacNone;

    /* scan along the specified direction until an unwanted tile is found */
    counter = 0;
    i = y;
    j = x;
    do
    {
        if ( i >= 0 && i < board->settings->height &&
             j >= 0 && j < board->settings->width )
        {
            /* count all tiles for current player */
            currentTile = board->tiles[i][j];
            if ( currentTile == tile )
            {
                counter++;
            }
        }

        i += yStep;
        j += xStep;
    }
    while ( i >= 0 && i < board->settings->height &&
            j >= 0 && j < board->settings->width &&
            currentTile == tile );


    /* come back to the starting point and reverse the direction */
    /* repeat the same process as above */
    i = y - yStep;
    j = x - xStep;
    do
    {
        if ( i >= 0 && i < board->settings->height &&
             j >= 0 && j < board->settings->width )
        {
            currentTile = board->tiles[i][j];
            if ( currentTile == tile )
            {
                counter++;
            }
        }

        i -= yStep;
        j -= xStep;
    }
    while ( i >= 0 && i < board->settings->height &&
            j >= 0 && j < board->settings->width &&
            currentTile == tile );

    return counter;
}



/******************************************************************************
 * Function: checkDraw                                                        *
 * Imports: pointer to the game board                                         *
 * Returns: TRUE if there are no emty tiles left on the board                 *
 *          FALSE if there are still some emty tiles left                     *
 * Exports: none                                                              *
 * Purpose: checks if there are no empty tiles left on the board, in which    *
 *          case the game ends in a draw unless a player has already won.     *
 ******************************************************************************/
BOOL checkDraw( GameBoard *board )
{
    int i, j;
    BOOL noCellsLeft = TRUE;

    /* iterate over each tile on the board
     * and stop when an emty tile is found */
    i = 0;
    while ( i < board->settings->height && noCellsLeft )
    {
        j = 0;
        while ( j < board->settings->width && noCellsLeft )
        {
            if ( board->tiles[i][j] == TicTacNone )
            {
                noCellsLeft = FALSE;
            }
            j++;
        }
        i++;
    }

    return noCellsLeft;
}



/******************************************************************************
 * Function: changePlayer                                                     *
 * Imports: pointer to the current player tile                                *
 * Exports: changes the current player value                                  *
 * Purpose: used to swap between players when changing turns                  *
 *          The algorithm increments the player value until it reaches the    *
 *          last player, when it resets back to 1 using modulo division       *
 ******************************************************************************/
void changePlayer( TicTacTile *player )
{
    *player = *player % ( TicTacTotal - 1 ) + 1;
}



/******************************************************************************
 * Function: deleteGameBoard                                                  *
 * Imports: pointer to game board                                             *
 * Exports: none                                                              *
 * Purpose: frees the memory allocated for a game board                       *
 ******************************************************************************/
void deleteGameBoard( GameBoard *board )
{
    int i;
    for ( i = 0; i < board->settings->height; i++ )
    {
        free( board->tiles[i] );
    }
    free( board->tiles );

    free( board );
}



/******************************************************************************
 * Function: deleteGameLog                                                    *
 * Imports: pointer to a game log                                             *
 * Exports: none                                                              *
 * Purpose: provides functionality to remove a game log from a linked list    *
 * Assertions: the imported pointer points to a GameLog struct                *
 ******************************************************************************/
void deleteGameLog( void *log )
{
    GameLog *gameLog = (GameLog*) log;
    freeList( gameLog->turnList, &deleteTurnLog );
    free( log );
}



/******************************************************************************
 * Function: deleteTurnLog                                                    *
 * Imports: pointer to a turn log                                             *
 * Exports: none                                                              *
 * Purpose: provides functionality to remove a turn log from a linked list    *
 * Assertions: the imported pointer points to a TurnLog struct                *
 ******************************************************************************/
void deleteTurnLog( void *log )
{
    free( log );
}



/******************************************************************************
 * Function: printGameLog                                                     *
 * Imports: pointer to output stream                                          *
 *          pointer to a game log                                             *
 * Exports: none                                                              *
 * Purpose: provides functionality to print a game log from a linked list     *
 * Assertions: the imported pointer points to a GameLog struct                *
 ******************************************************************************/
void printGameLog( FILE *stream, void *log )
{
    GameLog *gameLog = (GameLog*) log;
    #ifdef Editor
    fprintf( stream, "SETTINGS:\n"
                    "  M: %d\n"
                    "  N: %d\n"
                    "  K: %d\n\n",
                    gameLog->settings.width, gameLog->settings.height,
                    gameLog->settings.matches );
    #endif
    printList( stream, gameLog->turnList, &printTurnLog );
}



/******************************************************************************
 * Function: printTurnLog                                                     *
 * Imports: pointer to output stream                                          *
 *          pointer to a turn log                                             *
 * Exports: none                                                              *
 * Purpose: provides functionality to print a game log from a linked list     *
 * Assertions: the imported pointer points to a TurnLog struct                *
 ******************************************************************************/
void printTurnLog( FILE *stream, void *log )
{
    char tile;
    TurnLog *turnLog = (TurnLog*) log;
    switch ( turnLog->player )
    {
        case TicTacX:
            tile = 'X';
            break;
        case TicTacO:
            tile = 'O';
            break;
        default:
            tile = 'N';
    }

    fprintf( stream, "  Turn: %d\n"
                     "  Player: %c\n"
                     "  Location: %d,%d\n\n",
                     turnLog->turnNum, tile,
                     turnLog->location.x, turnLog->location.y );
}
