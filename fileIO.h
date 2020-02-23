/******************************************************************************
 * File:---- fileIO.h                                                         *
 * Author:-- Anton R                                                          *
 * Created:- 28/09/2019                                                       *
 * Modified: 20/10/2019                                                       *
 * Purpose:- Defines the maximum length of an input file and                  *
 *           the maximum length of an output file name.                       *
 *           Declares functions to read and validate game settings from an    *
 *           external file, as well as save game logs to a file.              *
 ******************************************************************************/

#ifndef FILEIO_H
#define FILEIO_H

#include "game.h"


/* max input file line length
 * set to an arbitrarily large value to enable meaningful output 
 * with large entry lines */
#define MAX_LINE_LEN 50


/* max log file name with all variables set to 2-digit numbers
 * (includes the null terminator) */
#define LOG_NAME_LEN 29


/* Function Prototypes */
int getSettings( char*, Settings* );
FILE *createLogFile( Settings*, char* );
int validSetting( char*, int );

#endif
