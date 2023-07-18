#include "main.h"

int main(__attribute__((unused))int ac, __attribute__((unused))char **av){

	ssize_t nchars;
	char *line = NULL;
	size_t size = 0;
	char *prompt;

	prompt = "(hbh) ";

	while (1){
		printf("%s", prompt);
		nchars = getline(&line, &size, stdin);
		if (nchars == -1 || strcmp(line, "exit\n") == 0 || feof(stdin))
		{
			printf("Exiting shell...\nbye\n");
			return (-1);
		}
		printf("%s", line);
	}

	free(line);
	return (0);
}
