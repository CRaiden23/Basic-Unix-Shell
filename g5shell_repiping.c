/* Name: Alex Luong
   Combination of both redirection and pipes
*/

#include "shell.h"

void Repiping(char *temp)
{
	int ifp, ofp, fd1[2], fd2[2];
	char *arg[10], *cmd[10], *cmd2[10], *cmd3[10], *cmd4[10];		//char array
	int i, a, j, cpipe = 0, left = 0, right = 0, status;			//counter
	pid_t pid;		//process id

	i = 0;
	
	char *token = strtok(temp, " ");	//separate the input out to use with exec()
	arg[i] = token;

	while(token != NULL)
	{
		i++;
		token = strtok(NULL, " ");		//Strtok is used to separate the char array 
		arg[i] = token;	
	}
	arg[i+1] = NULL;	//Last element need to be NULL for exec() to work

	for(a = 0; arg[a] != NULL; a++)     //loop through every single elements that is separated by " "
	{
		if(memcmp(arg[a],"<",1) == 0)       //if we have "<" then open input file
		{
			left++;
			ifp = open(arg[a+1], O_RDONLY);
		}
		else if(memcmp(arg[a],">",1) == 0)      //if we have ">" then open output
		{
			right++;
			ofp = open(arg[a+1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		}
		else if(memcmp(arg[a],"|",1) == 0)      //counting pipes
		{
			cpipe++;
		}
	}

	if(cpipe == 1)      //if there only one pipe in the command
	{
		if(left > 0 && right > 0)       // if there both "<" and ">" in the command
		{
			a = 0; j = 0;
			while(memcmp(arg[a],"<",1) != 0)       //get command 
			{
				cmd[j] = arg[a];
				a++; j++;
			}
			cmd[j] = NULL;
				
			a += 3; j = 0;		
			while(memcmp(arg[a],">",1) != 0)        //get command 
			{
				cmd2[j] = arg[a];
				a++; j++;
			}
			cmd2[j] = NULL;
			pipe(fd1);	            //open pipe
			if(fork() == 0)
			{
				dup2(ifp, fileno(stdin));           //open file -> command 1
				dup2(fd1[1], fileno(stdout));
				close(fd1[0]);
				close(fd1[1]);
				close(ifp);
				close(ofp);
				execvp(cmd[0], cmd);
			}
			else
			{
				if(fork() == 0)
				{
					dup2(fd1[0], fileno(stdin));        //command 1 -> command 2 -> output file
					dup2(ofp, fileno(stdout));
					close(fd1[0]);
					close(fd1[1]);
					close(ifp);
					close(ofp);
					execvp(cmd2[0], cmd2);		
				}
			}
			close(fd1[0]);          //close all pipes
			close(fd1[1]);
			close(ifp);
			close(ofp);
			
			for (i = 0; i < 3; i++)     //wait for childs
			{
    			wait(&status);
			}
		}
		else if(left > 0)       //if we only have "<"
		{
			a = 0; j = 0;
			while(memcmp(arg[a],"<",1) != 0)        //get command
			{
				cmd[j] = arg[a];
				a++; j++;
			}
			cmd[j] = NULL;
			
			a += 3; j = 0;		
			while(arg[a] != NULL)           //get command
			{
				cmd2[j] = arg[a];
				a++; j++;
			}
			cmd2[j] = NULL;
			pipe(fd1);	        //open pipe
			
			if(fork() == 0)
			{
				dup2(ifp, fileno(stdin));           //open file -> command
				dup2(fd1[1], fileno(stdout));
				close(fd1[0]);
				close(fd1[1]);
				close(ifp);
				execvp(cmd[0], cmd);
			}
			else
			{
				if(fork() == 0)
				{
					dup2(fd1[0], fileno(stdin));        //command 1 -> command 2
					close(fd1[0]);
					close(fd1[1]);
					close(ifp);
					execvp(cmd2[0], cmd2);		
				}
			}
			close(fd1[0]);          //close all pipes
			close(fd1[1]);
			close(ifp);
			
			for (i = 0; i < 3; i++)     //wait for child
			{
    			wait(&status);
			}
		}
		else if(right > 0)      //if we only have ">"
		{
			a = 0; j = 0;
			while(memcmp(arg[a],"|",1) != 0)        //get command
			{
				cmd[j] = arg[a];
				a++; j++;
			}
			cmd[j] = NULL;
				
			a += 1; j = 0;		
			while(memcmp(arg[a],">",1) != 0)        //get command
			{
				cmd2[j] = arg[a];
				a++; j++;
			}
			cmd2[j] = NULL;
			pipe(fd1);	            //open pipes
			
			if(fork() == 0)
			{
				dup2(fd1[1], fileno(stdout));       //command 1 -> command 2
				close(fd1[0]);
				close(fd1[1]);
				close(ofp);
				execvp(cmd[0], cmd);
			}
			else
			{
				if(fork() == 0)
				{
					dup2(fd1[0], fileno(stdin));        //command 2 -> output file
					dup2(ofp, fileno(stdout));
					close(fd1[0]);
					close(fd1[1]);
					close(ofp);
					execvp(cmd2[0], cmd2);		
				}
			}
			close(fd1[0]);      //close all pipes
			close(fd1[1]);
			close(ofp);
			
			for (i = 0; i < 3; i++)     //wait for child
			{
    			wait(&status);
			}
		}
	}
	else if(cpipe == 2)         //if we have 2 pipes
	{
		if(left > 0 && right > 0)       //again, if both "<" and ">"
		{
			a = 0; j = 0;
			while(memcmp(arg[a],"<",1) != 0)       //get command
			{
				cmd[j] = arg[a];
				a++; j++;
			}
			cmd[j] = NULL;
				
			a += 3; j = 0;	
			while(memcmp(arg[a],"|",1) != 0)        //get command
			{
				cmd2[j] = arg[a];
				a++; j++;
			}
			cmd2[j] = NULL;
			
			a += 1; j = 0;	
			while(memcmp(arg[a],">",1) != 0)        //get command
			{
				cmd3[j] = arg[a];
				a++; j++;
			}
			cmd3[j] = NULL;
			pipe(fd1);              //open pipes
			pipe(fd2);
			
			if(fork() == 0)
			{
				dup2(ifp, fileno(stdin));           //file -> command
				dup2(fd1[1], fileno(stdout));
				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);
				close(ifp);
				close(ofp);
				execvp(cmd[0], cmd);
			}
			else
			{
				if(fork() == 0)
				{
					dup2(fd1[0], fileno(stdin));
					dup2(fd2[1], fileno(stdout));       //command -> command
					close(fd1[0]);
					close(fd1[1]);
					close(fd2[0]);
					close(fd2[1]);
					close(ifp);
					close(ofp);
					execvp(cmd2[0], cmd2);		
				}
				else
				{
					if(fork() == 0)
					{
						dup2(fd2[0], fileno(stdin));    //command -> output file
						dup2(ofp, fileno(stdout));
						close(fd1[0]);
						close(fd1[1]);
						close(fd2[0]);
						close(fd2[1]);
						close(ifp);
						close(ofp);
						execvp(cmd3[0], cmd3);
					}
				}
			}
			
			close(fd1[0]);      //close all pipes
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
			close(ifp);
			close(ofp);
			
			for (i = 0; i < 3; i++)     //wait for child
			{
    			wait(&status);
			}
		}
		else if(left > 0)       // again, if we only have "<"
		{
			a = 0; j = 0;
			while(memcmp(arg[a],"<",1) != 0)        //get command
			{
				cmd[j] = arg[a];
				a++; j++;
			}
			cmd[j] = NULL;
			
			a += 3; j = 0;		
			while(memcmp(arg[a],"|",1) != 0)        //get command
			{
				cmd2[j] = arg[a];
				a++; j++;
			}
			cmd2[j] = NULL;
			
			a += 1; j = 0;
			while(arg[a] != NULL)           //get command
			{
				cmd3[j] = arg[a];
				a++; j++;
			}
			cmd3[j] = NULL;
			pipe(fd1);
			pipe(fd2);			
			
			if(fork() == 0)
			{
				dup2(ifp, fileno(stdin));           //file -> command
				dup2(fd1[1], fileno(stdout));
				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);
				close(ifp);
				execvp(cmd[0], cmd);
			}
			else
			{
				if(fork() == 0)
				{
					dup2(fd1[0], fileno(stdin));        //command -> command
					dup2(fd2[1], fileno(stdout));
					close(fd1[0]);
					close(fd1[1]);
					close(fd2[0]);
					close(fd2[1]);
					close(ifp);
					execvp(cmd2[0], cmd2);		
				}
				else
				{
					if(fork() == 0)
					{
						dup2(fd2[0], fileno(stdin));    //command -> command
						close(fd1[0]);
						close(fd1[1]);
						close(fd2[0]);
						close(fd2[1]);
						close(ifp);
						execvp(cmd3[0], cmd3);
					}
				}
			}
			
			close(fd1[0]);      //close all pipes
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
			close(ifp);
			
			for (i = 0; i < 3; i++)     //wait for child
			{
    			wait(&status);
			}
		}
		else if(right > 0)      //again, if we only have ">"
		{
			a = 0; j = 0;
			while(memcmp(arg[a],"|",1) != 0)        //get command
			{
				cmd[j] = arg[a];
				a++; j++;
			}
			cmd[j] = NULL;
				
			a += 1; j = 0;	
			while(memcmp(arg[a],"|",1) != 0)        //get command
			{
				cmd2[j] = arg[a];
				a++; j++;
			}
			cmd2[j] = NULL;
			
			a += 1; j = 0;	
			while(memcmp(arg[a],">",1) != 0)        //get command
			{
				cmd3[j] = arg[a];
				a++; j++;
			}
			cmd3[j] = NULL;
			
			pipe(fd1);
			pipe(fd2);
			
			if(fork() == 0)
			{
				dup2(fd1[1], fileno(stdout));       //command -> command
				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);
				close(ofp);
				execvp(cmd[0], cmd);
			}
			else
			{
				if(fork() == 0)
				{
					dup2(fd1[0], fileno(stdin));        //command -> command
					dup2(fd2[1], fileno(stdout));
					close(fd1[0]);
					close(fd1[1]);
					close(fd2[0]);
					close(fd2[1]);
					close(ofp);
					execvp(cmd2[0], cmd2);		
				}
				else
				{
					if(fork() == 0)
					{
						dup2(fd2[0], fileno(stdin));    //command -> output file
						dup2(ofp, fileno(stdout));
						close(fd1[0]);
						close(fd1[1]);
						close(fd2[0]);
						close(fd2[1]);
						close(ofp);
						execvp(cmd3[0], cmd3);
					}
				}
			}
			
			close(fd1[0]);      //close all pipes
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
			close(ofp);
			
			for (i = 0; i < 3; i++)         //wait for child
			{
    			wait(&status);
			}
		}
	}
	else
	{
		perror("Error! Only support up to two pipe!!!");       //print out error if more than 2 pipes (supported!) are typed in
	}
}