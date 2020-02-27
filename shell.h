#ifndef SHELL_H
#define SHELL_H

//Declare new headers here
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//Function Prototypes
// -- change directory
void ChangeDirectory(char *args);
void PrintWorkingDirectory();

// -- exit command
int ExitCommand();

// -- history
int writeHistory(char *cmd, int cmdCount, char *history);
	/*	Function: Stores cmd c-string into a given array at the cmdCount index
		Return  : 1 on success
	*/
void printHistory(int cmdCount, char *history);
	/*	Function: Outputs all cmdCount many commands that have been entered into the history
		Return  : void
	*/
// -- piping
void piper(char* cmd0, char* cmd1, char* cmd2, int dbl);
	/*	Function: Pipes cmd0 into cmd1 (and cmd1 into cmd2 if dbl != 0)
		Return  : void
	*/

// -- signal handling
void DisableSignals();
void RestoreSignals();

// -- Redirects and Piping
void Repiping(char *temp);
void redirection(char *command);
#endif
