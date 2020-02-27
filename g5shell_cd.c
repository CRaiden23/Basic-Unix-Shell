// Author: Aaron Hurley
// EUID:   adh0313

#include "shell.h"

/* 
	Change Directory system call
	Requires: argument to the new path (can be NULL)
*/
void ChangeDirectory(char *args)
{
	int status;
	
	// change directory system call
	// -- if it's NULL or empty, then go to the home directory
	// -- else we go to the path that args[1] is assigned to.
	if (args == NULL)
		status = chdir(getenv("HOME"));
	else
		status = chdir(args);
	//Error checking
	switch(status)
	{
		case ENOENT:
		case ENOTDIR:
			printf("No such directory\n");
		break;
	}
}
/*
	Prints out working directory
	Does not newline
*/
void PrintWorkingDirectory()
{
	char newwd[4096];
	getcwd(newwd, sizeof(newwd));
	printf("%s",newwd);
}
