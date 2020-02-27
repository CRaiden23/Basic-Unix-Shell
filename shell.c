/*  Group 5 
    Member: Steven Hurley, Alex Luong, Aaron Hurley and Oscar Garcia
    Instructor: Dr. Thompson
    Major 1: Make shell that support built-in command, redirection and pipes and combination of both
    
    
    ***COMBINATION NOTE!!!: Only run if the command is typed correctly*** 
        E.g. cmd < infile | cmd | cmd > outfile
            cmd < infile | cmd       or cmd | cmd > outfile
            cmd < infile | cmd | cmd or cmd | cmd | cmd > outfile
    ****Won't work for (e.g. cmd < file |) without any command given, will 
                return segfault****
*/

#include "shell.h"

int main(int argc, char **argv)
{
	pid_t pid;
	char input[512]; // this is what the user inputs [512 chars]
	char prompt[32] = "g5_major_1"; // this is the prompt>

	char * cmd[15]; // this is the input split into regular commands and piped; redirected commands [15 pure cmds/ piped/redirected cmds]
	char * indCmd[10]; // this is the individual command + 9 args [cmd + arg1 + ... + arg9]

	FILE *fp;
	if(argc == 2)
	{
		fp = fopen(argv[1],"r");
		if (fp == NULL)
			perror("Error opening file");
	}
	else if(argc > 3) // error print usage prompt
        {
                printf("USAGE: Batch Mode: ./a.out <filename>\n       Interactive Mode: ./a.out\n");
		exit(EXIT_FAILURE);
        }

	char history[20][512]; // history [20 cmds][512 chars]
	int cmdCount = 0; // total count of cmds entered

    	int flag = 0; // exit flag
	char temp[100]; // temporary string to pass into function call
	char command[100];
	int i, j; // counters for splitting up commands
	
	while(1) // interactive mode
	{
	    	// check for exit flag every time before the code 
		if(flag > 0)	// if the flag is raised by 1 (meaning previous group of command contain exit), exit
		{               
			printf("Exiting shell...\n");	//Print message
			exit(EXIT_SUCCESS);			//exit using exit()
		}
		//Set processor group here
		tcsetpgrp(fileno(stdin), getpgrp());
		
		//Disable signals on shell
		DisableSignals();
		
		if (argc == 1)
		{
			printf("\x1B[36m\n%s>\x1B[0m", prompt); // prompt
			gets(input); // grabs input
		}
		else
		{
			fgets(input, sizeof(input), fp);
		}

		if((argc == 2 && feof(fp))) // if user types "quit" 
		{
			exit(EXIT_SUCCESS); // exit
		}

		if(argc == 2)
	                printf("\nINPUT: \x1B[33m%s\x1B[0m\n",input);

		if(!strcmp(input,"chprompt")) // credit to Alex for this bit of code, I stole it from him (but made it better)
		{
			printf("Please enter the new prompt: ");
                        gets(prompt);
                        printf("Prompt change successful!\n");
			continue; // continue to restart the loop
		}

		cmdCount += writeHistory(input,cmdCount,(char*)history); // write everything to history

		i=1; // counter
		cmd[0] = strtok(input, ";\n"); // grab the first cmd
		while(i < 15) // grabs arguments
		{		     // NULL so we don't refill the line that we're pulling from
			cmd[i] = strtok(NULL, ";\n"); // separate cmds by semicolons
			i++;				// this way we have all the piped stuff together and the regular commands together
		}

		i = 0;
		while(cmd[i] != NULL) // while we have groups of commands to do
		{

			if(argc == 2)
				printf("\n\x1B[32m%s\x1B[0m\n",cmd[i]);

			j = 0;
			if(strchr(cmd[i], '|') == NULL && strchr(cmd[i], '<') == NULL && strchr(cmd[i], '>') == NULL ) // if we don't see anything
			{
				while(indCmd[j] = strtok_r(cmd[i], " ", &cmd[i]))
					j++;

				if (indCmd[0] != NULL) // makes sure empty spaces don't throw SEGV
				{
					pid = fork();
				 	if (pid == 0) // child
					{
						//Signal handling stuff
						setpgrp();
						tcsetpgrp(fileno(stdin), getpgid(pid));
						RestoreSignals();

						if(!strcmp(indCmd[0],"myhistory"))				// put built in functions here!!
							printHistory(cmdCount, (char*)history);
					
						/*path*/

						//Special case, if cd is picked up, don't execute execvp
						// -- it needs to happen to the parent process instead
						// -- it's also a built in command
						if(strcmp(indCmd[0],"cd") && strcmp(indCmd[0],"exit"))
						{
							execvp(indCmd[0], indCmd);
							printf( "\x1B[31m%s: command not found\n\x1B[0m",input ); // this will only run if execlp fails
						}

			                        exit(EXIT_FAILURE); // exit so we don't break out into the loop
					}
					else if (pid > 0) // parent
					{
						//Signal handling stuff
						setpgid(pid, pid);
						signal(SIGTTOU, SIG_IGN);
						tcsetpgrp(fileno(stdin), getpgid(pid));
						
						// Exit Command - outside the child process to avoid dealing with shared variable
                        			if(!strcmp(indCmd[0],"exit"))   // If there are exit then increase the flag
	                			{
	                        			flag += ExitCommand();
	                    			}

						//Change directory needs to happen onto the parent process, not the child
						if(!strcmp(indCmd[0],"cd"))
							ChangeDirectory(indCmd[1]);

						wait((int *)0);
					}
					else // error
					{
						perror("There was a forking error"); // print funny error
					}
				}
			}
			else if( strchr(cmd[i], '|') && strchr(cmd[i], '<') == NULL && strchr(cmd[i], '>') == NULL ) // if we see only a pipe somewhere
			{

				while(indCmd[j] = strtok_r(cmd[i], "|", &cmd[i]))
                                        j++;

				if( indCmd[1] && indCmd[2] == NULL ) // if we have two commands and not three
				{	       //CMD GROUP 0   ,  CMD GROUP 1    ,  CMD GROUP 2,    DOUBLE?
					piper( (char*)indCmd[0], (char*)indCmd[1], NULL, 0 );
				}
				else if( indCmd[2] && indCmd[3] == NULL ) // if we have three commands and not four
				{	      //CMD GROUP 0    ,  CMD GROUP 1    ,  CMD GROUP 2,    DOUBLE?
					piper( (char*)indCmd[0], (char*)indCmd[1], (char*)indCmd[2], 1 );
				}
				else if (indCmd[1] == NULL || indCmd[0] == NULL ) // this shouldn't ever happen
				{
					printf("Piping Syntax error!\n\tExpected: cmd1 | cmd2 [| cmd3]\n");
				}
				else // this happens if we have more than two pipes
				{
					printf("Piping error! Only two pipes supported in this shell\n");
				}
			}
			else if ( strchr(cmd[i], '|') == NULL && (strchr(cmd[i], '<') || strchr(cmd[i], '>')) ) // if we see only a redirect
			{
				printf("\t%s: Redirect only\n", cmd[i]);
				strcpy(command, cmd[i]);
			//	redirection(command);
			}
			else if( strchr(cmd[i], '|') && (strchr(cmd[i], '<') || strchr(cmd[i], '>')) ) // if we see a pipe AND a redirect
			{
				printf("\t%s: Redirect AND Pipe\n", cmd[i]);
				strcpy(temp, cmd[i]);
				Repiping(temp);
			}

			i++;			
		}
	}
}
