#include "main.h"

#define MAX_INPUT_LENGTH 100

void display_prompt() {
    printf("(hsh) ");
    fflush(stdout);
}

void execute_command(char* command) {
    
    int status;
    char *args[3];
    pid_t pid;
    
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Child process */
	args[0] = command;
	args[1] = NULL;
        if (execve(command, args, NULL) == -1) {
            printf("./hsh: 1: %s: not found\n", args[0]);
            exit(EXIT_FAILURE);
        }
    } else {
        /* Parent process */
        waitpid(pid, &status, 0);
    }
}

int main() {
    char *line = NULL;
    size_t bufsize = 0;
    ssize_t chars_read;

    while (1) {
        display_prompt();

        chars_read = getline(&line, &bufsize, stdin);

        if (chars_read == -1) {
            /* End of file (Ctrl+D) or error occurred */
            if (feof(stdin)) {
                printf("\n");
                break;
            } else {
                perror("getline");
                exit(EXIT_FAILURE);
            }
        }

        /* Remove the trailing newline character */
        line[strcspn(line, "\n")] = '\0';

        execute_command(line);
    }

    free(line);
    exit(EXIT_SUCCESS);
}
