/******************************************************************************
 * File:---- fileIO.c                                                         *
 * Author:-- Anton Rieutskyi                                                  *
 * Created:- 28/09/2019                                                       *
 * Modified: 10/10/2019                                                       *
 * Purpose:- This file defines the functions that handle reading and          *
 *           validating game settings from a file, as well as writing game    *
 *           logs to another file.                                            *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "fileIO.h"
#include "bool.h"
#include "game.h"
#include "interface.h"


/******************************************************************************
 * Function: getSettings                                                      *
 * Imports: fileName - name of the input file                                 *
 *          settings - pointer to the game settings struct                    *
 * Returns: TRUE if an error occurred while reading the file                  *
 *          FALSE if the file was read sucessfully                            *
 * Exports: the three values in settings: width, height, matches              *
 * Purpose: Attempts to read and validate the game settings from an external  *
 *          file. If successful, exports the values into a Settings struct    *
 *          and returns TRUE.                                                 *
 ******************************************************************************/
BOOL getSettings( char *fileName, Settings *settings )
{
    BOOL error;
    FILE *file;

    error = FALSE;

    /* open the input file */
    file = fopen( fileName, "r" );
    if ( file == NULL )
    {
        printErr( "Could not open the settings file" );
        error = TRUE;
    }
    else
    {
        int lineCount;
        int width, height, matches;
        lineCount = 0;
        width = 0;
        height = 0;
        matches = 0;
        
        /* start reading the input file */
        while ( !error && !feof( file ) && !ferror( file ) )
        {
            char *setting;
            int value;

            setting = (char*) malloc( MAX_LINE_LEN * sizeof( char ) );

            /* try to read values from a line and check for correct format */
            if ( fscanf( file, "%[^=]=%d\n", setting, &value ) != 2  )
            {
                printErr( "Invalid file format" );
                error = TRUE;
            }
            else
            {
                char *errString;
                errString = calloc( MAX_ERR_STRING * sizeof( char ), 1 );

                /* if format is correct, validate the setting */
                if ( validSetting( setting, value )  )
                {
                    if ( tolower( setting[0] ) == 'm' && width == 0  )
                    {
                        width = value;
                    }
                    else if ( tolower( setting[0] ) == 'n' && height == 0  )
                    {
                        height = value;
                    }
                    else if ( tolower( setting[0] ) == 'k' && matches == 0  )
                    {
                        matches = value;
                    }
                    else
                    {
                        sprintf( errString, "Duplicate setting: %s", setting );
                        printErr( errString );
                        error = TRUE;
                    }
                }
                else
                {
                    sprintf(
                        errString, "Invalid setting: %s=%d", setting, value );
                    printErr( errString );
                    error = TRUE;
                }

                free( errString );
            }

            free( setting );
            lineCount++;
        }

        /* if line count less than 3, then there are not enought settings */
        if ( !error && lineCount < 3 )
        {
            printErr( "Not all 3 settings were provided" );
            error = TRUE;
        }
        else if ( !error )
        {
            /* final validation and set the values */
            if ( width < matches || height < matches )
            {
                printErr( "Value of K is larger than the smallest dimension" );
                error = TRUE;
            }
            else
            {
                settings->width = width;
                settings->height = height;
                settings->matches = matches;
            }
        }

        fclose( file );
        file = NULL;
    }

    return error;
}


/******************************************************************************
 * Function: createLogFile                                                    *
 * Imports: settings of the current game                                      *
 *          pointer to a file name                                            *
 * Returns: pointer to an output file                                         *
 * Exports: name of the output file                                           *
 * Purpose: creates a log file based on the current date and time, as well as *
 *          the settings of the current game.                                 *
 *                                                                            *
 * Based on information from:                                                 *
 * https://www.tutorialspoint.com/c_standard_library/c_function_localtime.htm *
 * (last accessed 26/10/2019)                                                 *
 ******************************************************************************/
FILE *createLogFile( Settings *settings, char *fileName )
{
    FILE *logFile;
    time_t currentTime;
    struct tm *timeStruct;

    /* get current time from the system and convert it to a readable struct */
    time( &currentTime );
    timeStruct = localtime( &currentTime );

    /* create a file name based on the current time and game settings */
    sprintf( fileName, "MNK_%d-%d-%d_%02d-%02d_%02d-%02d.log",
        settings->width, settings->height, settings->matches,
        timeStruct->tm_hour, timeStruct->tm_min,
        timeStruct->tm_mday, timeStruct->tm_mon + 1 );

    /* try to open the output file */
    logFile = fopen( fileName, "w" );

    /* if successful, return the file pointer,
     * otherwise return NULL */
    if ( ferror( logFile ) )
    {
        printErr( "Failed to open the output file" );
        fclose( logFile );
        logFile = NULL;
    }

    return logFile;
}


/******************************************************************************
 * Function: validSetting                                                     *
 * Imports: setting - a string representing the name of the setting           *
 *          value - the current value of the setting                          *
 * Returns: TRUE if the setting has a valid name and is within a valid range  *
 *          FALSE otherwise                                                   *
 * Exports: none                                                              *
 * Purpose: ensures that the settings in the input file are valid             *
 ******************************************************************************/
BOOL validSetting( char *setting, int value )
{
    BOOL valid;

    valid = FALSE;
    if ( strlen( setting ) == 1 )
    {
        char ch;
        ch = tolower( setting[0] );
        if ( ch == 'm' || ch == 'n' || ch == 'k' )
        {
            if ( value > 0 && value <= MAX_DIMENSION)
            {
                valid = TRUE;
            }
        }
    }

    return valid;
}
