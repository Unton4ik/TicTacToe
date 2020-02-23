/******************************************************************************
 * File:---- main.c                                                           *
 * Author:-- Anton Rieutskyi                                                  *
 * Created:- 28/10/2019                                                       *
 * Modified: 20/10/2019                                                       *
 * Purpose:- the main program of the m-n-k tic-tac-toe game                   *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "fileIO.h"
#include "interface.h"

int main( int argc, char **argv )
{
    BOOL error;
    Settings   *gameSettings;

    gameSettings = (Settings*) malloc( sizeof(Settings) );

    /* ensure proper usage */
    if ( argc != 2 )
    {
        printf( "usage: TicTacToe settings\n" );
        error = TRUE;
    }
    else
    {
        /* try to get settings from the file */
        error = getSettings( argv[1], gameSettings );

        if ( !error )
        {
            /* print a welcome message and enter the main game loop */
            welcomeMessage( );
            gameLoop( gameSettings );
        }
        else
        {
            printf( "The settings file is invalid, exiting\n" );
            enterToContinue( );
        }
    }

    /* clean up */
    free( gameSettings );

    return (int) error;
}
