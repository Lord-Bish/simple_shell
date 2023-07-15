#include "main.h"
#include <string.h>

int main(int argc, char **argv, char **env)
{
	size_t size, nchars;
	char *line, *toke;
	char *token[10];
	int i, j;
	pid_t child_pid;
	int status;
	int count = 0;

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
		if (toke[0] == '\n')
			continue;
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
		if (strcmp(token[0], "env") == 0)
		{
			while(env[j] != NULL){
				printf("%s\n",env[j]);j++;}
			continue;
		}
		child_pid = fork();
		if(child_pid == 0)
		{
			if (execve(token[0], token, NULL) == -1)
				printf("%s: %i: %s: not found\n", argv[0], count, token[0]);
			return (0);
			break;
		}
		else
			wait(&status);
		count++;
	}
	return 1;
}
