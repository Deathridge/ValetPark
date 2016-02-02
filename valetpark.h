/* DO NOT CHANGE ANYTHING IN THIS FILE */

#ifndef _VALETPARK_H_
#define _VALETPARK_H_

/****************************************************************/
/* Standard includes */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/****************************************************************/
/* Constants and structure definitions */

/* Constant for the maximum size of the carpark */
#define	MAXCARPARKSIZE 12

/* Constant denoting an empty position in the carpark */
#define	EMPTY '.'

/* Constant denoting the target car in the carpark */
#define	TARGETCAR '#'

/* Enumeration for the possible movement directions */
typedef enum {NORTH, EAST, SOUTH, WEST} DIRECTION;

/* Enumeration for the possible car orientations */
typedef enum {NORTHSOUTH, EASTWEST} ORIENTATION;

/* Structure to hold information about a location within the carpark */
typedef struct
{
	/* The row number, starting from 0 */
	int row;
	/* The column number, starting from 0 */
	int col;
} LOCATION;

/* Structure to hold information about a car */
typedef struct
{
	/* The location of the front of the car.  As all cars are assumed
	   to be facing either north or west, this location represents
	   the northwest segment of the car */
	LOCATION front;
	/* The orientation of the car */
	ORIENTATION orientation;
	/* The length of the car */
	int length;
} CARINFO;

/* Structure to hold information about the state of the carpark */
typedef struct
{
	/* Rectangular grid of characters representing the position of
	   all cars in the game.  Each car appears precisely once in
	   the carpark */
	char grid[MAXCARPARKSIZE][MAXCARPARKSIZE];
	/* The number of rows used in carpark */
	int nRows;
	/* The number of columns used in carpark */
	int nCols;
	/* The location of the exit */
	LOCATION exit;
} CARPARK;

/****************************************************************/
/* Global variable definitions */

/* The carpark global variable */
extern CARPARK carpark;

/* DO NOT ADD ANY OTHERS */

/****************************************************************/
/* External function definitions */

/* The function that controls the graphical interface available under
   MacOSX in CSSE Lab 2.01 */
#if defined(__APPLE__)
extern void carparkGUI(void);
#endif

/****************************************************************/

#endif
