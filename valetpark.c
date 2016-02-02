/*
   CITS1210 Project 1
   Name:		<Betteridge, Daniel>
   Student number:	<20750196>
   Date:		<15/09/2011>
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "valetpark.h"

/****************************************************************/
/* Global variable definitions */
CARPARK carpark;

/* DO NOT ADD ANY OTHERS */

/****************************************************************/
/* Functions */

/*
   isValidLocation(l) returns true iff l represents a location within
   the bounds of the carpark.
*/
static bool isValidLocation(LOCATION l)
{
	if(l.row >= 0 && l.row < carpark.nRows)
	{
		if(l.col >= 0 && l.col < carpark.nCols)
		{
			return true;
		}
	}
	return false;	
}

/*
   isValidCarparkEntry(c) returns true iff c is a valid character that
   can appear in the "grid part" of a carpark-file.
*/
static bool isValidCarparkEntry(char c)
{
	if(c == EMPTY || c == TARGETCAR || (c >= 97 && c <= 122)){
		return true;
	}
	return false;
}

/*
   trimLine(line) removes any trailing new-line or carriage-return
   characters from the end of line.
*/
static void trimLine(char line[])
{
	int i =0;
	while(line[i] !='\0'){
		if(line[i] == '\r' || line[i] == '\n')
		{
			line[i] = '\0';
			break;
		}
		i = i+1;
	}
}

static LOCATION locationgen(int row, int col)
{
	LOCATION l;
	l.row = row;
	l.col = col;
	return l;
}


/*
   readCarparkFile(filename) reads the contents of the file named
   filename as a carpark-file, "filling" the global carpark variable with
   information read from the file.  The function terminates the program
   if the file is not a valid carpark-file or if the corresponding grid
   would be too large to store in the global carpark variable.
*/
static void readCarparkFile(char filename[])
{
	//Variables for opening and reading from the file
	FILE *Park;
	char read[BUFSIZ];		
	Park = fopen(filename, "r");
	int row;
	int col;
	
	//Check for null/empty file
	if(Park == NULL){
		printf("Fatal Error: cannot open file '%s'\n", filename);
		exit(EXIT_FAILURE);
		}
	
	//loop counters for retrieval method
	int readcount = 0;	
	
	if(fscanf(Park,"EXIT: %d %d\n", &row, &col) !=2)
	{
		printf("Invalid exit location");
		exit(EXIT_FAILURE);
	}else{
		carpark.exit.row = row;
		carpark.exit.col = col;
		readcount++;
	}
	
	while(fgets(read, sizeof read ,Park) != NULL)
	{		
		if(readcount != 0){ 
		trimLine(read);
		
		for(int i =0; read[i] != '\0'; i++){
		
		//ensures the carpark is not too large
		if(i>MAXCARPARKSIZE)
		{
			printf("Carpark too large");
			exit(EXIT_FAILURE);
		}
			
		if(i + 1> carpark.nCols)
		{
			carpark.nCols = i+1;
		}
		
			//Makes sure all characters are valid, and inserts them into the grid
		if(isValidCarparkEntry(read[i]))
		{
			carpark.grid[readcount - 1][i] = read[i];	
		}else{
			printf("Invalid characters in carpark"); 
			exit(EXIT_FAILURE);
			}
		}
		readcount = readcount + 1;		
		if((readcount - 1) > carpark.nRows){carpark.nRows = readcount - 1;}
		}			
	}				
	if(readcount > MAXCARPARKSIZE){printf("Carpark too large"); exit(EXIT_FAILURE);}
	if(!isValidLocation(carpark.exit)){printf("Invalid location for exit"); exit(EXIT_FAILURE);}	
	fclose(Park);
}

/*
   getValue(l) returns the character representing the car segment
   occupying location l in the carpark.  The function terminates the program
   if the location is not valid (i.e. outside the bounds of the carpark).
*/
static char getValue(LOCATION l)
{
	if(isValidLocation(l)){
		return carpark.grid[l.row][l.col];
	}else{		
		printf("Fatal Error: Outside bounds");
		exit(EXIT_FAILURE);
	}
}

/*
   setValue(l, value) sets location l in the carpark to value.
   The function terminates the program if the location is not valid
   (i.e. outside the bounds of the carpark).  Additionally, unless the
   function is used to "free-up" the location (set the location to EMPTY),
   the function also terminates the program if the location is already
   occupied by another car.
*/
static void setValue(LOCATION l, char value)
{
	if(isValidLocation(l)){
		if(carpark.grid[l.row][l.col] == '.'){
			carpark.grid[l.row][l.col] = value;
		}else if(value == '.'){
			carpark.grid[l.row][l.col] = value;
		}else{
			printf("Car occupies this location");
		}
	}else{printf("Invalid Location"); exit(EXIT_FAILURE);}	
}

/*
   printCarpark(void) prints the current contents of the carpark, printing
   the rectangular grid of characters held in the global carpark variable.
*/
static void printCarpark(void)
{
/* Uncomment the following preprocessor directive if you want the
   graphical interface under MacOSX in CSSE Lab 2.01 */
/*
#if defined(__APPLE__)
	carparkGUI();
#endif
*/
printf("Exit: %d %d\n", carpark.exit.row, carpark.exit.col);
for(int i=0; i< carpark.nRows; i++)
{
	for(int j=0; j< carpark.nCols; j++)
	{
		printf("%c",getValue(locationgen(i,j)));
	}
	printf("\n");
}
printf("\n");
}

/*
   isEmpty(l) returns true iff location l is empty (i.e. no car segment
   resides at that location in the grid).
*/
static bool isEmpty(LOCATION l)
{
	if(carpark.grid[l.row][l.col] == '.'){return true;}
	return false;
}

/*
   moveCarOneBay(car, d, c) attempts to move car by one bay in the
   direction of d, returning true iff the car was able to successfully
   complete the move.  The requested move is performed (and the carpark
   updated to reflect the move) iff the car can move in the given
   direction, the movement does not take the car outside the bounds of
   the carpark, and there is no other car at the destination location.
*/
static bool moveCarOneBay(char car, DIRECTION d, CARINFO c)
{	 	
	if (c.front.row == 0 && c.front.col == 0)
	{
		printf("Car not found in carpark");
		exit(EXIT_FAILURE);
	}
	
	if(c.orientation == NORTHSOUTH){
		if(d == NORTH && isEmpty(locationgen(c.front.row - 1,c.front.col)) && isValidLocation(locationgen(c.front.row - 1,c.front.col))){
			setValue(locationgen(c.front.row - 1,c.front.col),car);
			setValue(locationgen(c.front.row + c.length - 1,c.front.col),'.');
			c.front.row--;					
			return true;			
		}else if(d == SOUTH && isEmpty(locationgen(c.front.row + c.length,c.front.col)) && isValidLocation(locationgen(c.front.row + c.length,c.front.col))){
			setValue(locationgen(c.front.row + c.length,c.front.col),car);
			setValue(locationgen(c.front.row,c.front.col),'.');			
			c.front.row++;
			return true;
		}
	}else if(c.orientation == EASTWEST){
		if(d == EAST &&isEmpty(locationgen(c.front.row,c.front.col+c.length)) && isValidLocation(locationgen(c.front.row,c.front.col + c.length))){
			setValue(locationgen(c.front.row,c.front.col+c.length),car);
			setValue(locationgen(c.front.row,c.front.col),'.');			
			c.front.col++;
			return true;
		}else if(d == WEST &&isEmpty(locationgen(c.front.row,c.front.col-1)) && isValidLocation(locationgen(c.front.row,c.front.col-1))){
			setValue(locationgen(c.front.row,c.front.col-1),car);
			setValue(locationgen(c.front.row,c.front.col+(c.length -1)),'.');			
			c.front.col--;
			return true;		
		}
	}	
	return false;
}

/*
   findCar(car) locates car within the carpark, returning information
   about the location, orientation, and length of the car.  The function
   terminates the program if the car is not located within the carpark.
*/
static CARINFO findCar(char car)
{
	CARINFO c;
	c.length = 0;	
	bool check;
	//loop through and find northwest corner of the car, then end the loop and find orientation
	
	for(int i=0; i< carpark.nRows; i++)
	{
		for(int j=0; j< carpark.nCols; j++)
		{				
			if(getValue(locationgen(i,j)) == car)
			{					
				c.front.row = i;
				c.front.col = j;
				i = carpark.nRows;	
				j = carpark.nCols;	
				check = true;
			}
		}	
	}	
	if (check == false)
	{
		printf("Car not found in carpark");
		exit(EXIT_FAILURE);
	}
	//only need to check in positive direction, as the loop will already have covered the negative and obviously found nothing
	if(getValue(locationgen(c.front.row + 1,c.front.col)) == car)
	{
		c.orientation = NORTHSOUTH;
	}else if(getValue(locationgen(c.front.row,c.front.col + 1)) == car)
	{
		c.orientation = EASTWEST;
	}
	//loop in postive direction of orientation from front until car ends to find length
	if(c.orientation == NORTHSOUTH)
	{
		int i = 0;
		
		while(c.front.row + i < carpark.nRows && getValue(locationgen(c.front.row + i,c.front.col)) == car )
		{
			c.length++;
			i = i + 1;			
		}
	}else if(c.orientation == EASTWEST){
		int i = 0;
		while(c.front.col + i< carpark.nCols && getValue(locationgen(c.front.row,c.front.col + i)) == car )
		{
			c.length++;
			i= i +1;
		}
	}	
	
	return c;
}

/*
   move(car, d, amount) attempts to move car by amount in the direction
   of d, returning the actual number of grid cells the car is able to
   move in that direction.  The requested movement is performed (and
   the carpark updated to reflect the movement) iff the corresponding
   car can perform the requested move, otherwise no movement of the car
   is performed (and zero is returned).  The requested movement is also
   constrained to ensure the car does not collide with any other car
   and that the car remains within the bounds of the carpark.
*/
static int moveCar(char car, DIRECTION d, int amount)
{
	CARINFO c = findCar(car);	
	//keep track of the total number of moves that have been made for the given input
	int actual=0;	
	
	//Use orientation and direction then move in the correct direction by the given number of spaces, return total number of spaces moved.
	if(c.orientation == NORTHSOUTH){
		if(d == NORTH){
			for(int i=0; i<amount;i++ ){
				if(c.front.row - 1 >=0 && isEmpty(locationgen(c.front.row - 1,c.front.col)) && isValidLocation(locationgen(c.front.row - 1,c.front.col)) ){
					setValue(locationgen(c.front.row - 1,c.front.col), car);
					setValue(locationgen(c.front.row + c.length - 1,c.front.col), '.');
					c.front.row--;
					actual++;	
				}	
			}
			return actual;
			
		}else if(d == SOUTH){
			
			for(int i=0; i<amount;i++){
				if(c.front.row  < carpark.nRows && isEmpty(locationgen(c.front.row + c.length,c.front.col))&& isValidLocation(locationgen(c.front.row + c.length,c.front.col))){
					setValue(locationgen(c.front.row +c.length,c.front.col),car);
					setValue(locationgen(c.front.row,c.front.col), '.');
					c.front.row++;
					actual++;						
				}
			}
			return actual;
		}
	}else if(c.orientation == EASTWEST){
		if(d == WEST){
			for(int i=0; i<amount;i++){
				if(c.front.col - 1>=0 && isEmpty(locationgen(c.front.row,c.front.col-1)) && isValidLocation(locationgen(c.front.row,c.front.col-1))){
					setValue(locationgen(c.front.row,c.front.col-1), car);
					setValue(locationgen(c.front.row ,c.front.col + c.length - 1),'.');
					c.front.col--;
					actual++;				
				}
			}
			return actual;
		}else if(d == EAST){
			for(int i=0; i<amount;i++){
				if(c.front.row < carpark.nCols && isEmpty(locationgen(c.front.row,c.front.col+c.length))&& isValidLocation(locationgen(c.front.row,c.front.col+c.length))){
					setValue(locationgen(c.front.row ,c.front.col+c.length), car);
					setValue(locationgen(c.front.row,c.front.col), '.');
					c.front.col++;
					actual++;				
				}
			}
			return actual;
		}		
	}
	return 0;
}

/*
   hasExited(void) returns true iff the target car has exited (i.e. any
   segment of the target car is at the exit location).
*/
static bool hasExited(void)
{
	if(carpark.grid[carpark.exit.row][carpark.exit.col] == '#'){		
		return true;
	}
	return false;
}

/*
   printPrompt(void) prints the prompt for movement commands.
   This function is complete - DO NOT MODIFY IT.
*/
static void printPrompt(void)
{
	if (isatty(0))
	{
		static int moveno = 0;

		printf("move %d> ", ++moveno);
		fflush(stdout);
	}
}

/*
   processMoves(void) reads movement commands, processing each
   command, in turn to move the corresponding car within the carpark.
   Each movement command is constrained to ensure the corresponding car
   does not collide with any other car and that the car remains within
   the bounds of the carpark.  A movement command is performed (and the
   carpark updated to reflect the movement) iff the corresponding car
   can move in the given direction.  At the completion of each movement
   command (regardless of whether the command lead to actual movement),
   the current state of the carpark is printed.  The function completes
   immediately if a movement command leads to the target car exiting.
   The function terminates the program if any command is invalid.
*/
static void processMoves(void)
{	
	char alpha;
	char d;	
	DIRECTION dir;
	int amount;	
	char input[80];
	int movecount = 0;
	
	while(!hasExited()){
			//retrieves move command
			fgets(input, sizeof input, stdin);
			sscanf(input,"%c %c %d", &alpha, &d, &amount);		 
			
			//converts direction in capital letter form into DIRECTION variable
			if(d == 'N'){dir = NORTH;}
			else if(d == 'S'){dir = SOUTH;}
			else if(d == 'E'){dir = EAST;}
			else if(d == 'W'){dir = WEST;}
			else{printf("Fatal Error: Incorrect input for direction");
			exit(EXIT_FAILURE);
			}	
			
			//determines the correct method to use, then processes move command	
			if(amount == 1)
			{
				if(moveCarOneBay(alpha,dir,findCar(alpha)))
				{
					movecount++;
					printf("Processed move: %c %c %d ", alpha, d, amount);
					printPrompt();
					printf("\n");		
				}else{
					printf("Processed move: %c %c %d ", alpha, d, amount);
					printPrompt();
					printf("\n");
				}					
			}else{			
				if(moveCar(alpha,dir,amount) > 0)
				{				
					movecount++;
					printf("Processed move: %c %c %d ", alpha, d, amount);
					printPrompt();
					printf("\n");					
				}else{
					printf("Processed move: %c %c %d ", alpha, d, amount);
					printPrompt();
					printf("\n");						
				}
			}
			printCarpark();	
			if(hasExited())
			{
				printf("The target car is free after %d moves!\n", movecount);
			}
	}	
}

/****************************************************************/
/* The main function */

int main(int argc, char *argv[])
{
	/* Exit with an error if the the number of arguments (including
	   the name of the executable) is not 2 */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s carpark-file\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		/* Read the initial state of the carpark from file */
		readCarparkFile(argv[1]);
		printf("The initial carpark from '%s' is:\n", argv[1]);
		/* Print the contents of the carpark */
		
		printCarpark();
				
		/* Process user-provided moves, updating the state of
		   the carpark after each valid move */
				
		processMoves();
		
		/* Print the contents of the final carpark */
		printf("The final carpark is:\n");
		printCarpark();

		/* Finished successfully! */
		exit(EXIT_SUCCESS);
	}
	
	return 0;
}
