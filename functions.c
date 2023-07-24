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
 * excution - excute the normal commands
 * @command: command line with args
 * @args: the args of command
 * @pid: the child and parint forks
 * @status: wait status
 * @argv: the name of the file
 */
void excution(char *command, char **args, pid_t pid, int status, char *argv)
{
	char path[50];
	char comm[50];

	strcpy(path, "/usr/bin/");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	} else if (pid == 0)
	{
		args[0] = strtok(command, " ");
		args[1] = strtok(NULL, " ");
		args[2] = strtok(NULL, " ");
		args[3] = strtok(NULL, " ");
		if (args[0][0] !=  '/')
		{
			strcpy(comm, args[0]);
			args[0] = strcat(path, args[0]);
		}
		if (access(args[0], X_OK) != 0)
		{
			printf("%s: 1: %s: command not found\n", argv, comm);
			exit(EXIT_FAILURE);
		}
		if (execve(args[0], args, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	} else
	{
		waitpid(pid, &status, 0);
	}
}
