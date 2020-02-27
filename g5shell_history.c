#include "shell.h"


/*-----------------------\
|      Steven Hurley     |
|	  svh0040        |
|         Major 1   	 |
\-----------------------*/


int writeHistory(char *cmd, int cmdCount, char *history)
{
	// add cmd to an array of strings
	strncpy( history + 512*(cmdCount%20), cmd, 512);

	// increment count
	return 1;
}

void printHistory(int cmdCount, char *history)
{
	int i;
	int top;

	if(cmdCount > 20)
		top = 20;
	else
		top = cmdCount;

	for(i = 0; i < top; i++)
	// for however many commands we have in the counter
	{
		printf( "%d: %s\n", i+1, history+i*512 );
		// print out each line
	}

	exit(EXIT_SUCCESS);
}
