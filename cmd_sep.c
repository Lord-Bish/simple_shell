#include "main.h"

#define MAX_INPUT_LENGTH 100


/**
 * display_prompt - display prompt
 */
void display_prompt(void)
{
	printf("($) ");
	fflush(stdout);
}
/**
 * execute_command - the function that excute commands
 * @command: the command line
 * @argv: name of the file
 * @j: error code
 */
void execute_command(char *command, char *argv, int *j)
{
	char *args[11];
	int status;
	pid_t pid = 5;
	char *dir = "";
	char *env_var = "";
	char *env_val = "";
	int exit_status;
	char *status_str = "";

	if (strcmp(command, "exit") == 0)
	{
		free(command);
		exit(EXIT_SUCCESS);
	}
	else if (strncmp(command, "exit ", 5) == 0)
	{
		status_str = command + 5;
		exit_status = atoi(status_str);
		free(command);
		exit(exit_status);
	} else if (strncmp(command, "setenv ", 7) == 0)
		_setenv(command, env_var, env_val);
	else if (strncmp(command, "unsetenv ", 9) == 0)
	{
		env_var = strtok(command + 9, " ");
		if (env_var != NULL)
		{
			if (unsetenv(env_var) != 0)
				fprintf(stderr, "Failed to unset environment variable\n");
		} else
			fprintf(stderr, "Invalid unsetenv syntax\n");
	} else if (strncmp(command, "cd ", 3) == 0)
		_cd(command, dir);
	else if (strcmp(command, "env") == 0)
		print_env(environ);
	else
		ex(command, args, pid, &status, argv, j);
}
/**
 * main - the main program
 * @argc: number of args
 * @argv: array of args
 * Return: void
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t chars_read;
	char *end , *command;
	int j = 1;

	(void)argc;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();
		chars_read = getline(&line, &bufsize, stdin);
		if (chars_read == -1)
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			perror("getline");
			free(line);
			exit(EXIT_FAILURE);
		}
		if (chars_read == (ssize_t)-1)
			break;
		line[strcspn(line, "\n")] = '\0';
		command = strtok(line, ";");
		while (command != NULL)
		{
			while (*command == ' ')
				command++;
			end = command + strlen(command) - 1;
			while (end > command && *end == ' ')
			{
				end--;
			}
			*(end + 1) = '\0';
			execute_command(command, argv[0], &j);
			command = strtok(NULL, ";");
		}
	}
	free(line);
	exit(EXIT_SUCCESS);
}
