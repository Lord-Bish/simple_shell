#include "main.h"

/**
 * print_env - print the environ
 * @environ: the environ values
 */
void print_env(char **environ)
{

		char **env = environ;

		while (*env)
		{
			printf("%s\n", *env);
			env++;
		}
		return;
}

/**
 * _setenv - set env value or change it
 * @command: command line
 * @env_val: the env value
 * @env_var: the env variable
 */
void _setenv(char *command, char *env_val, char *env_var)
{
	env_var = strtok(command + 7, " ");
	env_val = strtok(NULL, " ");
	if (env_var != NULL && env_val != NULL)
	{
		if (setenv(env_var, env_val, 1) != 0)
		{
			fprintf(stderr, "Failed to set environment variable\n");
		}
	} else
	{
		fprintf(stderr, "Invalid setenv syntax\n");
	}
}

/**
 * _cd - change dirictory
 * @command: the whole command line
 * @dir: the dirictory
 */
void _cd(char *command, char *dir)
{
	dir = strtok(command + 3, " ");
	if (dir == NULL || strcmp(dir, "-") == 0)
	{
		dir = getenv("HOME");
		if (dir == NULL)
		{
			fprintf(stderr, "Failed to get home directory\n");
			return;
		}
	}
	if (chdir(dir) != 0)
	{
		fprintf(stderr, "Failed to change directory\n");
		return;
	}
	if (setenv("PWD", dir, 1) != 0)
	{
		fprintf(stderr, "Failed to update PWD environment variable\n");
	}
}

/**
 * ex - excute the normal commands
 * @command: command line with args
 * @args: the args of command
 * @pid: the child and parint forks
 * @argv: the name of the file
 * @j: error code
 * @env: local env
 */
void ex(char *command, char **args, pid_t pid, char *argv, int *j, char **env)
{
	int i;
	int status;
	char path[50];
	char comm[50];

	strcpy(path, "/usr/bin/");
	args[0] = strtok(command, " ");
	if (args[0][0] != '/')
		strcpy(comm, strcat(path, args[0]));
	else
		strcpy(comm, args[0]);
	for (i = 1; i <= 10; i++)
		args[i] = strtok(NULL, " ");
	if (access(comm, X_OK) != 0)
	{
		printf("%s: %i: %s: command not found\n", argv, (*j), args[0]);
		(*j)++;
		return;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(command);
		exit(EXIT_FAILURE);
	} else if (pid == 0)
	{
		if (execve(comm, args, env) == -1)
		{
			perror("execve");
			free(command);
			exit(EXIT_FAILURE);
		}
		free(command);
		exit(EXIT_SUCCESS);
		
	} else
	{
		waitpid(pid, &status, 0);
	}
}
