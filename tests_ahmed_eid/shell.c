#include "main.h"
#include <string.h>

int main(int argc, char **argv, char **env)
{
	size_t size;
	char *line;
	size_t nchars;
	char *token[10];
	int i = 0;
	char *toke;
	pid_t child_pid;
	int status;

	token[0] = "";
	while (strcmp(token[0], "exit") != 0)
	{
		i = 0;
		printf("($) ");
		nchars = getline(&line, &size, stdin);
		if (nchars == -1)
		{
			printf("\n");
			break;
		}
		toke = strtok(line, " ");
		if (toke[strlen(toke) - 1] == '\n')
			toke[strlen(toke) - 1] = '\0';
		token[i] = malloc(strlen(toke) + 1);
		strcpy(token[i], toke);
		while (toke != NULL)
		{
			i++;
			toke = strtok(NULL, " ");
			if (toke == NULL)
				break;
			if (toke[strlen(toke) - 1] == '\n')	
				toke[strlen(toke) - 1] = '\0';
			token[i] = malloc(strlen(toke) + 1);
			strcpy(token[i], toke);
		};
		token[i] = NULL;
		child_pid = fork();
		if(child_pid == 0)
		{
			if (execve(token[0], token, NULL) == -1)
				perror("Error:");
			break;
		}
		else
			wait(&status);
	}
	return 1;
}
