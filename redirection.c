/*Oscar Garcia*/
/*#include "shell.h"

void redirection(char *command)
{
	
	char *array[10], *com[10];
	int ifp, ofp, i = 0;
		// grep on text_pattern 
  	char *token = strtok(command, " ");	//used to be able to separate string 
	array[i] = token;

	while(token != NULL)
	{
		i++;
		token = strtok(NULL, " ");		 
		array[i] = token;	
	}
	array[i+1] = NULL;	

	i = 0;
	if (array[i] != NULL)
	{
		if(strcmp(array[i],"<") || strcmp(array[i],">"))  
		{
			break;
		}

		com[i] = array[i];
		i++;
	} 

	for(i = 0; array[i] != NULL; i++)
	{
		if(!strcmp(array[i],"<"))
		{
			ifp = open(array[i+1], O_RDONLY);
			dup2(ifp,0);
			close(ifp);
		}
	   else if(!strcmp(array[i],">"))
		{
			ofp = open(array[i+1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			dup2(ofp,1);
			close(ofp); 
		}
	}
	// execute
	execvp(com[0], com);
}
*/