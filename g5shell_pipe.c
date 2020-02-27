#include "shell.h"


/*-----------------------\
|      Steven Hurley     |
|	  svh0040        |
|         Major 1   	 |
\-----------------------*/

void piper(char* cmd0, char* cmd1, char* cmd2, int dbl)
{
        char * first[10];
        char * second[10];
        char * third[10]; // first, second, and third commands
        int i = 1;

        pid_t pidPar, pidChil, pidGChil; // pids for each of our forks
        int firToSec[2], secToThi[2]; // file descriptors for pipes

        // firToSec[0] is the read end of the first two commands SECOND COMMAND READS FROM THIS
        // firToSec[1] is the write end of the first two commands  FIRST COMMAND WRITES TO THIS

        // secToThi[0] is the read end of the last two commands THIRD COMMAND READS FROM THIS
        // secToThi[1] is the write end of the last two commands SECOND COMMAND WRITES TO THIS

        pipe(firToSec); // create pipe from first to second
        pipe(secToThi); // create pipe from second to third

        first[0] = strtok(cmd0, " "); // grab the command
        while(i < 10)
        {
                first[i] = strtok(NULL, " "); // split the first command into its arguments
                i++;
        }

        i = 1;
        second[0] = strtok(cmd1, " "); // grab the command
        while(i < 10)
        {
                second[i] = strtok(NULL, " "); // split the second command into its arguments
                i++;
        }

        i = 1;
        third[0] = strtok(cmd2, " "); // grab the command
        while(i < 10)
        {
                third[i] = strtok(NULL, " "); // split the third command into its arguments
                i++;
        }

        pidPar = fork();
        if( pidPar == 0 ) // child
        {
                // this makes the first commands output go to the next command instead of stdout
                dup2(firToSec[1], fileno(stdout));

                close(firToSec[0]); // close all pipes before we do exec
                close(firToSec[1]);
                close(secToThi[0]);
                close(secToThi[1]);

                execvp(first[0], first);
                printf( "%s: command not found\n",cmd0 );
                exit(1);
        }
        else if( pidPar > 0) // parent
        {
                pidChil = fork();
                if( pidChil == 0 ) // child
                {       // read from the output of previous instead of stdin
                        dup2(firToSec[0], fileno(stdin));

                        if(dbl)// write to input of next
                                dup2(secToThi[1], fileno(stdout));

                        close(firToSec[0]); // close all pipes before we do exec
                        close(firToSec[1]);
                        close(secToThi[0]);
                        close(secToThi[1]);

                        execvp(second[0], second);
                        printf( "%s: command not found\n",cmd1 );
                        exit(1);
                }
                else if( pidChil > 0 ) // parent
                {
                        if(dbl) // if we have a double pipe
                        {
                                pidGChil = fork();
                                if( pidGChil == 0 ) // child
                                {       // read from input of previous
                                        dup2(secToThi[0], fileno(stdin));

                                        close(firToSec[0]); // close all pipes before we do exec
                                        close(firToSec[1]);
                                        close(secToThi[0]);
                                        close(secToThi[1]);

                                        execvp(third[0], third);
                                        printf( "%s: command not found\n",cmd2 );
                                        exit(1);
                                }
                                else if( pidGChil < 0 )// error
                                {
                                        perror("There was a forking error");
                                }
                        }
                }
                else // error
                {
                        perror("There was a forking error");
                }
        }
        else // error
        {
                perror("There was a forking error");
        }

        close(firToSec[0]); // close everything
        close(firToSec[1]);
        close(secToThi[0]);
        close(secToThi[1]);

        for (i = 0; i < 3; i++) // wait for this to iterate three times
        {
                wait((int*)0); // this'll just chill until a child terminates, incrementing the for loop
        }
}
