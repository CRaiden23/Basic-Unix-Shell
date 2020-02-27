//Author: Aaron Hurley
//EUID:   adh0313

#include "shell.h"

void DisableSignals()
{
	//Variables
	struct sigaction sig;
	int i;

	//Set to ignore signals
	sig.sa_handler = SIG_IGN;

	
	for(i = 1; i <= 33; i++)
	{
		if((i != SIGKILL) && (i != SIGSTOP) && (i != 32) & (i != 33))
			assert(sigaction(i, &sig, NULL) == 0);
	}
}

void RestoreSignals()
{
	//Variables
	struct sigaction sig;
	int i;

	//Set to ignore signals
	sig.sa_handler = SIG_DFL;


	for(i = 1; i <= 33; i++)
	{
		if((i != SIGKILL) && (i != SIGSTOP) && (i != 32) & (i != 33))
			assert(sigaction(i, &sig, NULL) == 0);
	}
}
